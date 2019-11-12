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

// Второй этап: запуск методов локальной минимизации в попытках улучшить результат: 
std::vector<std::pair<Real, Vector>>
find_local_mins_with_threads(Function f, const StopCondition& stop_condition, const std::vector<std::pair<Real, Vector>>& inData);

void* add_points_to_queue(void* args);

void* calc_f_with_threads(void* args);

struct producerArgs {
    // Shared variables
    std::queue<Vector>& readQueue;
    std::queue<Vector>& writeQueue;
    pthread_mutex_t& consumerMutex;
    pthread_mutex_t& producerMutex;
    pthread_cond_t& canProduce;
    pthread_cond_t& canConsume;
    bool& eof;

    // Own variables
    const Vector& min;
    const Vector& max;
};

struct consumerArgs {
    // Shared variables
    std::queue<Vector>& readQueue;
    std::queue<Vector>& writeQueue;
    pthread_mutex_t& consumerMutex;
    pthread_mutex_t& producerMutex;
    pthread_cond_t& canProduce;
    pthread_cond_t& canConsume;
    bool& eof;

    pthread_mutex_t& writeMutex;
    std::set<std::pair<Real, Vector>>& candidates;

    // Own variables
    const Function f;
    const uint32_t nBestPoints;
};

std::vector<std::pair<Real, Vector>>
find_absmin(Function f, const StopCondition& stop_condition, uint32_t dim, uint32_t nBestPoints, uint32_t nAllPoints, const Vector& min, const Vector& max);
