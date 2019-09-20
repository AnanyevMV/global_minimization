#pragma once

/* Tools */
#include "Data.h"
#include "Math.h"


/**
 *
 * @struct RmsProp
 * @brief Root Mean Square Propagation method
 * @param Function f
 * Минимизируемая функция
 * @param Vector startPoint
 * Точка старта минимизации
 * @param Vector parameters
 * Параметры метода
 * @param Real grad_accuracy
 * Точность расчета градиента в методе
 * @param int iter_limit
 * Максимальное число итераций метода
 *
*/


IterationData RmsProp(Function f, Vector startPoint, Vector parameters, Real grad_accuracy, int iter_lim);
