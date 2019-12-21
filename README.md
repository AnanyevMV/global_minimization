# Глобальная минимизация многомерной функции

Пространство поиска заполняется равномерной сеткой, в каждом узле которой параллельно вычисляется значение целевой функции. Составляется список кандидатов (k точек, значения целевой функции в которых минимально), а затем из каждого кандидата параллельно запускаются алгоритмы поиска локального минимума. Возвращается информация об глобальном минимуме (самая минимальная точка), об лучшей точке для каждого метода, об лучшем методе для каждой точке, а также об всех результатах (информация по каждому методу минимизации и по каждой точке).
Информация возвращается в структуре GlobalMinimizationResults.
Глобальный минимум в ней хранится в поле GlobalMinimum.
Информация выводится в консоль, а также сохраняется в файл Global_Minimization_Results.txt

## Виды сеток:
* Последовательности Нидеррайтера (b=2)

## Алгоритмы локальной минимизации:
1. BFGS
2. Hessian Free
3. Slow hessian free
4. Nesterov
5. DFP
6. Powell
7. Adadelta
8. Adagrad
9. Adam
10. AdaMax
11. AdamW
12. AMSGrad
13. BFGS 2
14. DFP 2
15. Nadam
16. Powell 2
17. Powell 21
18. RmsProp

В методах минимизации есть дубликаты, но их реализация немного отличается.
Было принято решение их оставить.

## Как скачать:
Нажмите на кнопку "Clone or download" и скачайте архив с проектом  
Или введите следующую команду в терминале
```
git clone https://github.com/hellishsatan/global_minimization.git  
```

## Как собрать и запустить
Скачиваем и устанавливаем CMake https://cmake.org/  
В папке с репозиторием создаём папку Release и перемещаемся в неё. Запускаем утилиту cmake.
```
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release .. -G "Unix Makefiles"
```
cmake по-умолчанию может генерировать .sln file для Visual Studio (для пользователей Windows)  
или, например, проект Xcode (для пользователей Mac OS)  
Это зависит от настроек. Поэтому мы явно указываем, что генерируем Makefile.  

Запускаем утилиту make
```
make
```
После сборки запускаем программу
```
./global_minimization
```

Для Debug-сборки
```
cmake .. -G "Unix Makefiles"
```
Либо можно явно указать  
```
cmake -DCMAKE_BUILD_TYPE=Debug .. -G "Unix Makefiles"
```

Также можно собирать проект в какой-то IDE, открыв репозиторий как CMake проект.  
Для пользователей Windows предлагается использовать для этого Microsoft Visual Studio.  
Ниже есть инструкция по подключению библиотеки Pthreads.  
В репозитории находится .dll файл (для пользователей Windows x64)  
А также в папке lib находятся необходимые .h файлы, и .lib файлы  
Требуются минимальные действия, необходимо следовать инструкции ниже
## Как пользоваться:
```
Главная функция
GlobalMinimizationResults
find_absmin(Function f, const StopCondition& stop_condition, uint32_t dim, uint32_t nBestPoints, uint32_t nAllPoints, uint32_t miniBatchSize, uint32_t maxQueueSize, const Vector& min, const Vector& max,uint32_t HowManyConsumers, bool checkBorders);

// Её возвращаемое значение - структура
struct GlobalMinimizationResults {
    std::vector<std::vector<IterationData> > AllResults;
    std::vector<IterationData> BestMethodForEachPoint;
    std::vector<IterationData> BestPointForEachMethod;
    IterationData GlobalMinimum;
};

// У структуры IterationData следующие поля
struct IterationData {
    Vector x_initial;         // Начальная точка
    Real f_initial;           // Значение функции в начальной точке
    Vector x_prev;            // Предущыщая точка
    Real   f_prev;            // Значение функции в предыдущей точке
    Vector x_curr;            // Текущая точка    
    Real   f_curr;            // Значение функции в текущей точке
    int   iter_counter;      // Количество итераций
    std::string method_title; // Название метода
    ...
    ...
    ...
    // Можно передавать поток std::cout для вывода в консоль или, например, std::ofstream для вывода в файл
    void print(std::ostream& os){
        os << "-----------------------------------------------------" << std::endl;
        os << "Starting point: " << x_initial << std::endl;
        os << "Function value at the starting point: " << f_initial << std::endl;
        os << "Previous point: " << x_prev << std::endl;
        os << "Function value at the previous point: " << f_prev << std::endl;
        os << "Current point: " << x_curr << std::endl;
        os << "Function value at the current point: " << f_curr << std::endl;
        os << "Number of iterations: " << iter_counter << std::endl;
        os << "Method title: " << method_title << std::endl;
        os << "-----------------------------------------------------" << std::endl;
    }
};
```

Пример использования:
```
// Функция Букина N 6; Глобальный минимум f(-10,1) = 0
Real BukinN6_function(const Vector& x){
    return 100*(abs(x[1]-0.01*x[0]*x[0]))+0.01*abs(x[0]+10);
}

int main() {
    auto start = std::chrono::steady_clock::now();
    // Границы
    Vector min = {-100,-100};
    Vector max = {100,100};
    // Размерность пространства
    uint32_t dim = 2;
    // Количество лучших точек
    uint32_t nBestPoints = 100;
    // Количество точек, которое необходимо сгенерировать
    uint32_t nAllPoints = 1000000;
    // Размер мини-партии, которое необходимо генерировать за раз
    // Если параметр равен единице, то генерируем по одной точке
    uint32_t miniBatchSize = 128;
    // Максимальный размер каждой из двух очередей (+ miniBatchSize - 1)
    // Максимальный размер может быть превышен максимум на miniBatchSize - 1
    uint32_t maxQueueSize = 4096;

    // узнаем количество ядер
    uint32_t nCores = std::max(1u, std::thread::hardware_concurrency());

    // Определяем сколько будет потоков-потребителей

    // Если ядер больше 4, 1 ядро оставляем ОС
    uint32_t HowManyConsumers = nCores > 4 ? nCores - 2 : std::max(1u, nCores - 1);

    // Для простых для вычисления функций скорость работы при одном потоке-потребителе может быть выше
    // Чем при нескольких, т.к. потоки будут постоянно засыпать и просыпаться
    // Потому что функция очень быстро считается. И много потребителей обсчитают всё быстрее чем новые точки успеют появиться.
    // tms-сеть генерирует точки медленнее чем считается функция в них. Даже если генерировать большими партиями.
    // Конкретно для функции выше следует сделать всего один поток потребитель.
    // Но, очевидно, что для сложных функций следует использовать больше потоков-потребителей.
    // В этом можно убедиться, добавив пустой цикл (с volatile переменной, чтобы компилятор ничего не оптимизировал) в функцию.
    // Получится имитация сложной для вычисления функции.
    // for (volatile int i = 0; i < 100000; ++i) {};

    // uint32_t HowManyConsumers = 1;

    // Если флаг равен true, то GlobalMinimum, BestPointForEachMethod и BestMethodForEachPoint
    // будут проверены на границы min и max. Если точка вылезла за границы, она не будет учитываться при поиске минимумов
    bool checkBorders = false;

    std::cout << "Program started" << std::endl;
    auto results = find_absmin(BukinN6_function, default_stop_condition, dim, nBestPoints, nAllPoints, miniBatchSize, maxQueueSize, min, max, HowManyConsumers, checkBorders);
    std::ofstream outputFile("Global_Minimization_Results.txt");
    std::cout << "Computation completed" << std::endl;
    std::cout << "\t\t\t\t    Global Minimum" << std::endl;
    std::cout << "-----------------------------------------------------" << std::endl;
    outputFile << "\t\t\t\t    Global Minimum" << std::endl;
    outputFile << "-----------------------------------------------------" << std::endl;
    results.GlobalMinimum.print(std::cout);
    results.GlobalMinimum.print(outputFile);
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << "\t\t\t Best point for each method" << std::endl;
    std::cout << "-----------------------------------------------------" << std::endl;

    outputFile << "-----------------------------------------------------" << std::endl;
    outputFile << "\t\t\t Best point for each method" << std::endl;
    outputFile << "-----------------------------------------------------" << std::endl;
    for (unsigned int i = 0; i < results.BestPointForEachMethod.size(); i++){
        results.BestPointForEachMethod[i].print(std::cout);
        results.BestPointForEachMethod[i].print(outputFile);
    }
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << "\t\t\t Best method for each point" << std::endl;
    std::cout << "-----------------------------------------------------" << std::endl;

    outputFile << "-----------------------------------------------------" << std::endl;
    outputFile << "\t\t\t Best method for each point" << std::endl;
    outputFile << "-----------------------------------------------------" << std::endl;
    for (unsigned int i = 0; i < results.BestMethodForEachPoint.size(); i++){
        results.BestMethodForEachPoint[i].print(std::cout);
        results.BestMethodForEachPoint[i].print(outputFile);
    }
    std::cout << "-----------------------------------------------------" << std::endl;
    outputFile << "-----------------------------------------------------" << std::endl;
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;
    outputFile.close();
    return 0;
}
```

## Технические требования
Для компиляции и сборки программы, необходимо иметь:
* компилятор, поддерживающий C++17;
* 64-битный процессор с 2 и более ядрами; (подмодуль irrpoly подмодуля tms-nets требует как минимум 2 ядра)
* библиотеку Pthreads
* (желательно) утилиту CMake версии 3.8 и выше.

### Инструкция по подключению Pthreads на Windows для Microsoft Visual Studio

Первым делом необходимо подредактировать файл CMakeLists.txt  
Там нужно раскомментировать строчки. Там указано, какие именно.  
Просто необходимо убрать символ "#" в начале необходимых строк.  

https://sourceware.org/pub/pthreads-win32/dll-latest/  
Здесь можно скачать файлы, необходимые для библиотеки Pthreads  
Необходимо в папке lib выбрать разрядность ОС (32 или 64) и скачать файл pthreadVC2.lib  
Необходимо в папке dll выбрать разрядность ОС (32 или 64) и скачать файл pthreadVC2.dll  
Необходимо из папки include скачать файлы pthread.h, semaphore.h, sched.h  

Необходимо поместить pthreadVC2.lib в папку lib проекта  
Необходимо три .h файла поместить в папку lib/include проекта  

.dll файл необходимо поместить рядом с .exe файлом  
Например, для Visual Studio .exe файл будет находиться примерно в этих папках:  
C:\Users\User\Documents\git\global_minimization\out\build\x64-Debug  
C:\Users\User\Documents\git\global_minimization\out\build\x64-Release  

До первого build'a .exe файла ещё не будет. Необходимо сделать первый build.  
И создастся .exe файл, и высветится ошибка, что не найден .dll файл.  
И осталось только закинуть .dll файл в папку с .exe файлом.  

В проекте уже есть необходимые файлы для пользователей 64-битной ОС Windows.  
.lib файл в нужном месте, .h файлы тоже  
.dll файл находится в папке с проектом  
Его необходимо скопировать и вставить рядом с exe файлом.  
Если .exe файла еще нет, то необходимо сделать build и он где-то появится.  
И в папку с .exe файлом необходимо засунуть .dll файл  
