#pragma once

#include <functional>

#include "Tools/math.hpp"

struct IterationData;
typedef std::function<bool(const IterationData&)> StopCondition;

// Структура, хранящая информацию о текущей итерации:
struct IterationData {
    Vector x_initial;         // Начальная точка
    Real f_initial;           // Значение функции в начальной точке
    Vector x_prev;            // Предущыщая точка
    Real   f_prev;            // Значение функции в предыдущей точке
    Vector x_curr;            // Текущая точка
    Real   f_curr;            // Значение функции в текущей точке
    int   iter_counter;      // Количество итераций
    std::string method_title; // Название метода

    IterationData(); // Конструктор по-умолчанию

    IterationData(Vector x_initial, Real f_initial, Vector x_prev, Real f_prev, Vector x_curr, Real f_curr, int iter_counter, std::string method_title) :
            x_initial(x_initial), f_initial(f_initial), x_prev(x_prev), f_prev(f_prev), x_curr(x_curr), f_curr(f_curr), iter_counter(iter_counter), method_title(method_title){};



    void print(std::ostream& os) {
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

    void next(const Vector& x_next, const Real f_next); // Переход к следующей итерации
};

typedef IterationData(*Method)(Function, Vector, const StopCondition&);
// Условие остановы итерации, применяемое по-умолчанию:
bool default_stop_condition(const IterationData&);
