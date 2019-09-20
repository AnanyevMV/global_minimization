#pragma once

/* Tools */
#include "Data.h"
#include "Math.h"

#include <math.h>

/**
 *
 * @struct Nadam
 * @brief Nesterov-accelerated Adaptive Moment Estimation (Nadam)
 * @param Nadam::f
 * Минимизируемая функция
 * @param Nadam::startPoint
 * Точка старта минимизации
 * @param Nadam::parameters
 * Параметры метода
 * @param Nadam::grad_accuracy
 * Точность расчета градиента в методе
 * @param Nadam::iter_limit
 * Максимальное число итераций метода
 *
*/


IterationData Nadam(Function f, Vector startPoint, Vector parameters, Real grad_accuracy, int iter_lim);
