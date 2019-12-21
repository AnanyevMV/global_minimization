#include "Tools/global_min.hpp"


#include <iostream>
#include <chrono>

// Тестовые функции:
Real f1(const Vector& x) {
    return 1 + x[0] + x[1] - x[0] * x[1] + x[0] * x[0] + x[1] * x[1];
}

Real f2(const Vector& x) {
    return 1 + 7 * x[0] + 5 * x[1] + 0.5 * x[0] * x[1] + 3 * x[0] * x[0] + x[1] * x[1];
}

Real f3(const Vector& x) {
    return 100 + 7 * x[0] + 5 * x[1] - 10 * x[0] * x[1] + 3 * x[0] * x[0] + 10 * x[1] * x[1];
}

Real f4(const Vector& x) {
    return 100 + 7 * x[0] + 5 * x[1] - 10.95 * x[0] * x[1] + 3 * x[0] * x[0] + 10 * x[1] * x[1];
}

Real f5(const Vector& x) {
    return 1+x[0]+x[1]+x[2] + x[0] * x[1] + x[0] * x[2] + x[1] * x[2] + x[0] * x[0] + x[1] * x[1] + x[2] * x[2];
}

Real f6(const Vector& x) {
    return 10 * std::pow(x[0], 4) + 15 * std::pow(x[1], 4) + 15 * x[0] * x[1];
}

Real f7(const Vector& v) {
    Real x = v[0];
    Real y = v[1];
    return 10 * std::pow(x, 6) + 15 * std::pow(y, 6) - 20 * (std::pow(x, 3) * y + x * std::pow(y, 3));
}

Real f8(const Vector& v) {
    auto x = v[0], y = v[1];
    return std::pow(x, 6) + std::pow(y, 6) - 2 * (std::pow(x, 3) * y + x * std::pow(y, 3)) + x*x + y*y;
}

Real f9(const Vector& v) {
    auto x = v[0], y = v[1];
    return std::pow(x, 6) + std::pow(y, 6) - 3 * (std::pow(x, 3) * y + x * std::pow(y, 3)) + x*x + y*y;
}

Real f10(const Vector& v) {
    auto x = v[0], y = v[1];
    return std::pow(x, 6) + std::pow(y, 6) - 2 * (std::pow(x, 3) * y + x * std::pow(y, 3)) + std::pow(x, 4) + std::pow(y, 4) - x*x - y*y;
}

Real f11(const Vector& v) {
    Real fun = std::pow(v[0] - 1, 2) / 4;
    for (unsigned int i=1; i < v.size(); ++i) {
        fun += std::pow(v[i] - 2 * std::pow(v[i-1], 2) + 1, 2);
    }
    return fun;
}

Real f12(const Vector &v) {
    Real fun = std::pow(v[0] - 1, 2) / 4;
    for (unsigned int i=1; i < v.size(); ++i) {
        fun += std::abs(v[i] - 2 * std::pow(v[i-1], 2) + 1);
    }
    return fun;
}

Real f13(const Vector &v) {
    Real fun = std::abs(v[0] - 1) / 4;
    for (unsigned int i=1; i < v.size(); ++i) {
        fun += std::abs(v[i] - 2 * std::pow(v[i-1], 2) + 1);
    }
    return fun;
}

Real f14(const Vector &v) {
    Real fun = 0;
    for (unsigned int i=1; i < v.size(); i += 2) {
        fun += 100 * std::pow(std::pow(v[i-1], 2) - v[i], 2) + std::pow(v[i-1] - 1, 2);
    }
    return fun;
}

Real f15(const Vector &v) {
    Real fun = 0;
    for (unsigned int i=1; i < v.size(); i += 2) {
        fun += 10 * std::abs(std::pow(v[i-1], 2) - v[i]) + std::abs(v[i-1] - 1);
    }
    return fun;
}

Real f16(const Vector &v) {
    Real fun = 0;
    for (unsigned int i=0; i < v.size(); ++i) {
        fun += std::pow(v[i], 2);
    }
    return fun;
}

Real f17(const Vector &v) {
    Real fun = 10 * v.size();
    for (unsigned int i=0; i < v.size(); ++i) {
        fun += std::pow(v[i] - 10 * std::cos(2 * M_PI * v[i]), 2);
    }
    return fun;
}

Real f18(const Vector &v) {
    return std::pow(std::pow(v[0], 2) + v[1] - 11, 2) + std::pow(std::pow(v[1], 2) + v[0] - 7, 2);
}

Real f19(const Vector &v) {
    const int b[] = {8, 18, 44, 144};
    Real fun = 0;
    Real g_fun;
    for (int i = 0; i < 4; ++i) {
        g_fun = 0;
        for (int j = 0; j < 4; ++j) {
            g_fun += 1;
            g_fun *= v[i];
        }
        g_fun -= b[i];
        fun += std::pow(g_fun, 2);
    }
    return fun;
}

Real f20(const Vector &v) {
    return std::pow(v[1] - 5.1 / (4 * M_PI_2) * std::pow(v[0], 2) + 5 * v[0] / M_PI - 6, 2) +
           10 * (1 - 1 / (8 * M_PI)) * std::cos(v[0]) + 10;
}

Real f21(const Vector &v) {
    return std::sin(v[0] + v[1]) + std::pow(v[0] - v[1], 2) - 1.5 * v[0] + 2.5 * v[1] + 1;
}

Real f22(const Vector &v) {
    return 0.26 * (std::pow(v[0], 2) + std::pow(v[1], 2)) - 0.48 * v[0] * v[1];
}


Real __function(const Vector& x){
    return 100*(abs(x[1]-0.01*x[0]*x[0]))+0.01*abs(x[0]+10);
}

Real __function2(const Vector& x){
    return -(x[0]*x[0] + x[1]*x[1]);
}

Real __function3(const Vector& x){
    return x[0]*x[0] - 7*x[0] + 3;
}

Real __function1(const Vector& x){
    return x[0]*x[0] + x[1]*x[1] - 3*x[0];
}

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
