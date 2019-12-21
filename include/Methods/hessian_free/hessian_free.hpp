#pragma once

#include "Tools/math.hpp"
#include "Tools/StopCondition.hpp"

// Быстрый (модифицированный) алгоритм Hessian Free
IterationData hessian_free(Function f, Vector x, const StopCondition& stop_condition = default_stop_condition);
// f - указатель на целевую функцию
// x - начальное приближение
// stop_condition - критерий остановы
// Результат работы метода будет лежать в структуре данных о последней итерации

// Медленный алгоритм Hessian Free (без модификации)
IterationData slow_hessian_free(Function f, Vector x, const StopCondition& stop_condition = default_stop_condition);
// f - указатель на целевую функцию
// x - начальное приближение
// stop_condition - критерий остановы
// Результат работы метода будет лежать в структуре данных о последней итерации

// Апроксимация умножения градиента функции f в точке х на вектор dx
// Погрешность O(||h||^2*||dx||^3), где h - выбранный шаг дифференцирования
Real grad_prod_vect(Function f, const Vector& x, const Vector& dx);

// Апроксимация умножения матрицы Гессе в точке x на вектор dx
// Погрешность O(||h||^2*||dx||^3)
Vector hess_prod_vect(Function f, const Vector& x, const Vector& dx);

// Метод сопряженных градиентов
Vector conjugade_gradient(Matrix A, Vector b, Vector x);
