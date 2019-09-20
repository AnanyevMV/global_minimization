#pragma once

/* Tools */
#include "Data.h"
#include "Math.h"


/**
*
* @struct Adadelta
* @brief Adaptive Moment Estimation method
* @param Adadelta::f
* Минимизируемая функция
* @param Adadelta::startPoint
* Точка старта минимизации
* @param Adadelta::parameters
* Параметры метода
* @param Adadelta::grad_accuracy
* Точность расчета градиента в методе
* @param Adadelta::iter_limit
* Максимальное число итераций метода
*
*/


IterationData Adadelta(Function f, Vector startPoint, Vector parameters, Real grad_accuracy, int iter_lim);
