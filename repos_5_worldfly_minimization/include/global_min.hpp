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

// Первый этап: вычисление значений функции в точках сетки:
// Автор: Козырев Дмитрий
//std::vector<std::pair<Real, Vector>>
//calc_f_with_threads(Function f, const std::vector<Vector> & inData);

// Второй этап: запуск методов локальной минимизации в попытках улучшить результат: 
// Автор: Козырев Дмитрий
std::vector<std::pair<Real, Vector>>
find_local_mins_with_threads(Function f, const StopCondition& stop_condition, const std::vector<std::pair<Real, Vector>>& inData);

// Основная функция поиска абсолютных минимумов:
// Автор: Козырев Дмитрий
std::vector<std::pair<Real, Vector>>
find_absmin(Function f, const StopCondition& stop_condition, uint32_t dim, uint32_t nBestPoints, uint32_t nAllPoints, Vector min, Vector max);

void* add_points_to_queue(void *args);

void* my_calc_f_with_threads(void* args);

struct producerArgs{
	std::queue<Vector> &queueOfPoints;
	std::set<std::pair<Real, Vector>> &candidates;
	pthread_mutex_t &queueMutex;
	pthread_cond_t &queueCondAddMore;
	pthread_cond_t &queueCondEndOfFile;
	Vector &min;
	Vector &max;
	volatile bool &eof;
};

struct consumerArgs{
    Function f;
	std::queue<Vector> &queueOfPoints;
	std::set<std::pair<Real, Vector>> &candidates;
	pthread_mutex_t &queueMutex;
	pthread_mutex_t &writeMutex;
	pthread_cond_t &canProduce;
	pthread_cond_t &canConsume;
	volatile bool &eof;
};

void
my_find_absmin(Function f, const StopCondition& stop_condition, uint32_t dim, uint32_t nBestPoints, uint32_t nAllPoints, Vector min, Vector max);
