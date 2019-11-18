#include "global_min.hpp"

#include <array>

#include "thread_pool.h"

constexpr int MaxQueueSize = 2048;


std::vector<std::pair<Real, Vector>>
find_local_mins_with_threads(Function f, const StopCondition& stop_condition,
                             const std::vector<std::pair<Real, Vector>>& inData) {
    // Создаем вектор под ответ
    std::vector<std::pair<Real, Vector>> outData(inData.size());

    // Количество потоков
    const uint32_t cores = ThreadPool::GetHardwareCoresAmount();

    // Создаем пул потоков
    ThreadPool thread_pool{cores};

    // Массив функций минимизации
    static const std::array<std::function<IterationData(Function, Vector, const StopCondition&)>, 6> min_funcs{
        &bfgs, &hessian_free, &slow_hessian_free, &nesterov, &dfp, &powell
    };

    // Мьютекс на запись
    pthread_mutex_t outWrite;
    pthread_mutex_init(&outWrite, nullptr);

    for (size_t i = 0; i < inData.size(); ++i) {
        const auto& data = inData[i];

        // Записываем начальное значение
        outData[i] = data;

        for (const auto& min_func : min_funcs) {
            thread_pool.Add([&, data, i]() {
                const auto iter_data = min_func(f, data.second, stop_condition);
                const auto x_curr = iter_data.x_curr;

                const auto f_curr = iter_data.f_curr;
                const auto pair = std::make_pair(f_curr, x_curr);

                // Записываем ответ
                pthread_mutex_lock(&outWrite);

                // Заменяемым минимальным
                outData[i] = std::min(outData[i], pair);

                pthread_mutex_unlock(&outWrite);
            });
        }
    }

    thread_pool.CreateThreads();
    thread_pool.WaitForAll();

    return outData;
}


// tms-сети возвращают нам вектора, компоненты которых принадлежат [0,1)
void __tmp_tms_result_imitation(unsigned int n, unsigned int numOfDimension) {
    std::ofstream output_file;
    output_file.open("tms-result-imitation.txt", std::ios::app);
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < numOfDimension; j++) {
            if (j != numOfDimension - 1) {
                output_file << uniform(0, 1) << " ";
            }
            else {
                output_file << uniform(0, 1);
            }
        }
        if (i != n - 1) {
            output_file << std::endl;
        }
    }
    output_file.close();
}

/**
 * \brief Меняет очереди местами (оба мьютекса должны быть залочены перед вызовом)
 * \tparam T producerArgs или consumerArgs
 * \param argsStruct Указатель на структуру аргументов
 * \param writeQueueEmpty [out, optional] Указывает, является ли writeQueue пустой очередью после обмена
 * \return false - если обе очереди пустые, иначе - true
 */
template <typename T>
bool SwapQueues(T* argsStruct, bool* writeQueueEmpty = nullptr) {
    bool result = true;

    if (argsStruct->writeQueue.empty() && argsStruct->readQueue.empty()) {
        result = false;
    }
    else {
        // Меняем очереди местами
        std::swap(argsStruct->writeQueue, argsStruct->readQueue);
    }

    if (writeQueueEmpty)
        *writeQueueEmpty = argsStruct->writeQueue.empty();

    return result;
}

/**
 * \brief Проверяет, являются ли обе очереди пустыми
 * \tparam T producerArgs или consumerArgs
 * \param argsStruct Указатель на структуру аргументов
 * \param lockProducer Нужно ли лочить поставщика
 * \return true - если обе очереди пустые, иначе - false
 */
template <typename T>
bool AreQueuesEmpty(T* argsStruct, bool lockProducer) {
    if (lockProducer)
        pthread_mutex_lock(&argsStruct->producerMutex);

    const bool result = argsStruct->writeQueue.empty() && argsStruct->readQueue.empty();

    if (lockProducer)
        pthread_mutex_unlock(&argsStruct->producerMutex);

    return result;
}

/**
 * \brief Проверяет, являются ли обе очереди полными
 * \tparam T producerArgs или consumerArgs
 * \param argsStruct Указатель на структуру аргументов
 * \param lockProducer Нужно ли лочить поставщика
 * \return true - если обе очереди полные, иначе - false
 */
template <typename T>
bool AreQueuesFull(T* argsStruct, bool lockProducer) {
    if (lockProducer)
        pthread_mutex_lock(&argsStruct->producerMutex);

    const bool result = argsStruct->writeQueue.size() >= MaxQueueSize && argsStruct->readQueue.size() >= MaxQueueSize;

    if (lockProducer)
        pthread_mutex_unlock(&argsStruct->producerMutex);

    return result;
}

// Поток потребитель
void* calc_f_with_threads(void* args) {
    volatile consumerArgs* consArgs = static_cast<consumerArgs*>(args);

    while (true) {
        pthread_mutex_lock(&consArgs->consumerMutex);

        if (consArgs->readQueue.empty()) {
            pthread_mutex_lock(&consArgs->producerMutex);

            bool isWriteQueueEmpty;
            const bool result = SwapQueues(consArgs, &isWriteQueueEmpty);

            // Если очередь поставщика пуста (после свапа), то сигналим ему
            if (isWriteQueueEmpty) {
                pthread_cond_signal(&consArgs->canProduce);
            }

            // Если обмен был неуспешным (обе очереди пусты)
            if (!result) {
                // Проверяем, будут ли еще элементы
                if (consArgs->eof) {
                    // Если нет, то выходим из цикла (и из функции)
                    pthread_mutex_unlock(&consArgs->producerMutex);
                    pthread_mutex_unlock(&consArgs->consumerMutex);
                    break;
                }

                //  Если будут, то ждем
                while (AreQueuesEmpty(consArgs, false)) {
                    if (consArgs->eof)
                        break;

                    consArgs->producerCanSignal = true;

                    pthread_mutex_unlock(&consArgs->producerMutex);
                    pthread_cond_wait(&consArgs->canConsume, &consArgs->consumerMutex);
                    pthread_mutex_lock(&consArgs->producerMutex);
                }

                pthread_mutex_unlock(&consArgs->producerMutex);

                // В идеале, анлочить его не нужно здесь
                // А то там он в цикле снова захватится
                pthread_mutex_unlock(&consArgs->consumerMutex);

                continue;
            }

            pthread_mutex_unlock(&consArgs->producerMutex);
        }

        // Берем элемент из очереди
        Vector point = consArgs->readQueue.front();
        consArgs->readQueue.pop();

        pthread_mutex_unlock(&consArgs->consumerMutex);

        // Выполняем функцию
        auto res = consArgs->f(point);

        // Сохраняем результат

        pthread_mutex_lock(&consArgs->writeMutex);

        consArgs->candidates.emplace(res, point);
        if (consArgs->candidates.size() > consArgs->nBestPoints) {
            consArgs->candidates.erase(std::prev(consArgs->candidates.end()));
        }

        pthread_mutex_unlock(&consArgs->writeMutex);
    }

    pthread_exit(nullptr);

    return nullptr;
}

std::vector<std::string> split_string(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());

    return results;
}

// Функция для треда-поставщика
void* add_points_to_queue(void* args) {
    volatile producerArgs* ptrProducerArgs = static_cast<producerArgs*>(args);

    // Открываем файл
    std::ifstream file{"tms-result-imitation.txt"};

    std::string str;

    while (true) {
        bool at_eof = false;
        if (std::getline(file, str)) {
            // Сплитим строку по пробелам, получаем вектор строк results
            const std::vector<std::string> results = split_string(str);

            Vector v(results.size());
            for (unsigned int i = 0; i < results.size(); ++i) {
                // std::stold приводит строку к long double
                // Приводим точку на единичном s-мерном кубе к точке в s-мерном прямоугольном параллелепипеде
                v[i] = std::stold(results[i]) * (ptrProducerArgs->max[i] - ptrProducerArgs->min[i]) + ptrProducerArgs->min[i];
            }

            // Сохраняем точку
            pthread_mutex_lock(&ptrProducerArgs->producerMutex);

            ptrProducerArgs->writeQueue.push(v);

            // Если есть спящие треды, то сигналим тредам-потребителям, что в очереди появились элементы
            if (ptrProducerArgs->producerCanSignal) {
                // Предотвращаем deadlock
                pthread_mutex_unlock(&ptrProducerArgs->producerMutex);

                // Лочим в правильном порядке
                pthread_mutex_lock(&ptrProducerArgs->consumerMutex);
                pthread_mutex_lock(&ptrProducerArgs->producerMutex);

                ptrProducerArgs->producerCanSignal = false;
                pthread_cond_broadcast(&ptrProducerArgs->canConsume);

                pthread_mutex_unlock(&ptrProducerArgs->consumerMutex);
            }

            pthread_mutex_unlock(&ptrProducerArgs->producerMutex);
        }
        else {
            at_eof = true;
        }

        if (ptrProducerArgs->writeQueue.size() >= MaxQueueSize || at_eof) {
            pthread_mutex_lock(&ptrProducerArgs->consumerMutex);
            pthread_mutex_lock(&ptrProducerArgs->producerMutex);

            // Обновим eof
            ptrProducerArgs->eof = at_eof;

            // Если больше точек не будет, то сигналим всем потребителям (на случай если они спят)
            if (at_eof) {
                pthread_cond_broadcast(&ptrProducerArgs->canConsume);
            }

            // Если обе очереди полные
            if (AreQueuesFull(ptrProducerArgs, false)) {
                pthread_mutex_unlock(&ptrProducerArgs->consumerMutex);

                // Ожидаем
                pthread_cond_wait(&ptrProducerArgs->canProduce, &ptrProducerArgs->producerMutex);

                // Чтобы предотвратить возможный deadlock
                pthread_mutex_unlock(&ptrProducerArgs->producerMutex);
                // Мы лочим в одинаковом порядке, как и треды-потребители
                // Иначе может быть deadlock
                pthread_mutex_lock(&ptrProducerArgs->consumerMutex);
                pthread_mutex_lock(&ptrProducerArgs->producerMutex);
            }

            // Меняем очереди местами
            SwapQueues(ptrProducerArgs);

            pthread_mutex_unlock(&ptrProducerArgs->producerMutex);
            pthread_mutex_unlock(&ptrProducerArgs->consumerMutex);

            // Если элементов больше нет, то выходим
            if (at_eof) {
                break;
            }
        }
    }

    file.close();

    pthread_exit(nullptr);

    return nullptr;
}


std::vector<std::pair<Real, Vector>>
find_absmin(Function f, const StopCondition& stop_condition, uint32_t dim, uint32_t nBestPoints, uint32_t nAllPoints, const Vector& min,
            const Vector& max) {
    // Несколько проверок на входные данные:
    assert(dim > 0u && dim == min.size() && dim == max.size());
    assert(nBestPoints <= nAllPoints && nBestPoints > 0u);
    for (uint32_t i = 0; i < dim; ++i) {
        assert(min[i] <= max[i]);
    }

    // Создаем 2 очереди
    std::queue<Vector> readQueue;
    std::queue<Vector> writeQueue;

    // Мьютекс потребителей
    pthread_mutex_t consumerMutex;
    pthread_mutex_init(&consumerMutex, nullptr);

    // Мьютекс поставщика
    pthread_mutex_t producerMutex;
    pthread_mutex_init(&producerMutex, nullptr);

    // Мьютекс для потребителей для записи результатов
    pthread_mutex_t writeMutex;
    pthread_mutex_init(&writeMutex, nullptr);

    // Условная переменная "можно производить"
    pthread_cond_t canProduce;
    pthread_cond_init(&canProduce, nullptr);

    // Условная переменная "можно потреблять"
    pthread_cond_t canConsume;
    pthread_cond_init(&canConsume, nullptr);

    // Формирование списка лучших кандидатов
    std::set<std::pair<Real, Vector>> candidates;

    // bool переменная, отвечающая за то, закончился ли файл
    bool eof = false;

    bool producerCanSignal = false;

    pthread_t producer;

    // Создаем экземпляр структуры аргументов поставщика
    producerArgs provider_args{
        readQueue, writeQueue, consumerMutex, producerMutex, canProduce, canConsume, eof, producerCanSignal, min, max
    };

    // Создаем экземпляр структуры аргументов потребителя
    consumerArgs consumer_args{
        readQueue, writeQueue, consumerMutex, producerMutex, canProduce, canConsume, eof,
        producerCanSignal, writeMutex, candidates, f, nBestPoints
    };

    pthread_create(&producer, nullptr, &add_points_to_queue, static_cast<void*>(&provider_args));

    // узнаем количество ядер
    const uint32_t nCores = std::max(1u, std::thread::hardware_concurrency());

    // Если ядер > 4, то 1 ядро оставляем системе
    // Иначе задействуем все ядра (1 ядро поставщику)
    const uint32_t howManyConsumers = nCores > 4 ? nCores - 2 : nCores - 1;

    // Вектор идентификаторов тредов-потребителей
    std::vector<pthread_t> vectOfConsumersTids(howManyConsumers);

    for (uint32_t i = 0; i < howManyConsumers; ++i) {
        pthread_t tid;
        pthread_create(&tid, nullptr, &calc_f_with_threads, static_cast<void*>(&consumer_args));
        vectOfConsumersTids.push_back(tid);
    }

    pthread_join(producer, nullptr);

    for (auto tid : vectOfConsumersTids) {
        pthread_join(tid, nullptr);
    }

    // ----- Второй этап: запуск алгоритмов поиска локального минимума из выбранных точек -----
    // Подготовка (перекладываем точки из set в vector - возможен рост скорости при последовательном размещении в памяти точек):
    std::vector<std::pair<Real, Vector>> temp(candidates.begin(), candidates.end());

    // Многопоточная обработка кандидатов:
    auto answer = find_local_mins_with_threads(f, stop_condition, temp);

    // Итоговая сортировка всех найденных точек по неубыванию значения функции в них:
    std::sort(answer.begin(), answer.end());

    return answer;
}
