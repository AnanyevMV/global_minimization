#pragma once

#include "Tools/math.hpp"
#include "Tools/StopCondition.hpp"
#include "../../Tools/Parameters.hpp"

// Метод Нестерова (возвращается результат - точка минимума и количество сделанных итераций)
IterationData nesterov(Function f, Vector startingPoint, const StopCondition& stop_condition = default_stop_condition);
// f - указатель на целевую функцию
// startingPoint - начальное приближение
// stop_condition - критерий остановы
// Результат работы метода будет лежать в структуре данных о последней итерации
