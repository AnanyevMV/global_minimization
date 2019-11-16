#pragma once

#include "sobolseqgenerator.hpp"
#include "math.hpp"
#include "nesterov.hpp"
#include "hessian_free.hpp"
#include "bfgs.hpp"
#include "dfp.hpp"
#include "powell.hpp"
#include "StopCondition.hpp"

#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <thread>
#include <pthread.h>

// Имитация результата работы tms-ников. n - количество точек, numOfDimension - размерность
// Результат - это файл "tms-result-imitation.txt"
// Если вызвать функцию снова, то новые данные добавятся в конец
void __tmp_tms_result_imitation(unsigned int n, unsigned int numOfDimension);

// Второй этап: запуск методов локальной минимизации в попытках улучшить результат: 
std::vector<std::pair<Real, Vector>>
find_local_mins_with_threads(Function f, const StopCondition& stop_condition, const std::vector<std::pair<Real, Vector>>& inData);

// Функция для треда-поставщика
// Он поставляет точки в writeQueue, размер очереди 2048 элементов
// Если очередь заполнится, он свапает очереди (readQueue и writeQueue) и продолжает заполнять
// Если обе очереди заполнены, то он засыпает до пробуждения.
// Его пробуждают треды-потребители, когда очередь поставщика опустеет при свапе
void* add_points_to_queue(void* args);

// Функция для тредов-потребителей
// Она считает многопоточно целевую функцию для сгенерированных tms-сетью точек
// Она берет точки из readQueue, когда очередь кончится она readQueue и writeQueue свапает и сигналит треду-поставщику поставлять
// Она сохраняет в std::set лучшие точки (их количество - nBestPoints)
void* calc_f_with_threads(void* args);

// Аргументы для треда-поставщика передаются через структуру
struct producerArgs {
    // Shared variables
    std::queue<Vector>& readQueue;
    std::queue<Vector>& writeQueue;
    pthread_mutex_t& consumerMutex;
    pthread_mutex_t& producerMutex;
    pthread_cond_t& canProduce;
    pthread_cond_t& canConsume;
    bool& eof;
    bool& producerCanSignal;

    // Own variables
    const Vector& min;
    const Vector& max;
};

// Аргументы для тредов-потребителей передаются через структуру
struct consumerArgs {
    // Shared variables
    std::queue<Vector>& readQueue;
    std::queue<Vector>& writeQueue;
    pthread_mutex_t& consumerMutex;
    pthread_mutex_t& producerMutex;
    pthread_cond_t& canProduce;
    pthread_cond_t& canConsume;
    bool& eof;
    bool& producerCanSignal;

    pthread_mutex_t& writeMutex;
    std::set<std::pair<Real, Vector>>& candidates;

    // Own variables
    const Function f;
    const uint32_t nBestPoints;
};

// Главная функция, которая ищет глобальный минимум
// Она сама вызовет все функции выше
// Она возвращает вектор пар <целевая функция, точка>
// Размер вектора - nBestPoints
std::vector<std::pair<Real, Vector>>
find_absmin(Function f, const StopCondition& stop_condition, uint32_t dim, uint32_t nBestPoints, uint32_t nAllPoints, const Vector& min, const Vector& max);
