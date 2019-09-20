#pragma once

/* Tools */
#include "Data.h"
#include "Math.h"


/**
 *
 * @struct Adagrad
 * @brief Adaptive Gradient method
 * @param Adagrad::f
 * Минимизируемая функция
 * @param Adagrad::startPoint
 * Точка старта минимизации
 * @param Adagrad::parameters
 * Параметры метода
 * @param Adagrad::grad_accuracy
 * Точность расчета градиента в методе
 * @param Adagrad::iter_limit
 * Максимальное число итераций метода
 *
*/

IterationData Adagrad(Function f, Vector startPoint, Vector parameters, Real grad_accuracy, int iter_lim);
