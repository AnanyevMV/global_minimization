#pragma once

#include "Tools/math.hpp"
#include "Methods/Adadelta/Adadelta.h"
#include "Methods/Adagrad/Adagrad.h"
#include "Methods/Adam/Adam.h"
#include "Methods/AdaMax/AdaMax.h"
#include "Methods/Adam/AdamW.h"
#include "Methods/AMSGrad/AMSGrad.h"
#include "Methods/bfgs/bfgs.hpp"
#include "Methods/bfgs/bfgs2.hpp"
#include "Methods/dfp/dfp.hpp"
#include "Methods/dfp/dfp2.hpp"
#include "Methods/hessian_free/hessian_free.hpp"
#include "Methods/Nadam/Nadam.h"
#include "Methods/nesterov/nesterov.hpp"
#include "Methods/powell/powell.hpp"
#include "Methods/powell/powell2.hpp"
#include "Methods/powell/powell21.hpp"
#include "Methods/RmsProp/RmsProp.h"
#include "Tools/StopCondition.hpp"

#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <thread>
#include <pthread.h>

// Второй этап: запуск методов локальной минимизации в попытках улучшить результат: 
std::vector<std::vector<IterationData> >
find_local_mins_with_threads(Function f, const StopCondition& stop_condition, const std::vector<std::pair<Real, Vector>>& inData);

// Функция для треда-поставщика
// Он поставляет точки в writeQueue, размер очереди 2048 элементов
// Если очередь заполнится, он свапает очереди (readQueue и writeQueue) и продолжает заполнять
// Если обе очереди заполнены, то он засыпает до пробуждения.
// Его пробуждают треды-потребители, когда очередь поставщика опустеет при свапе
void* add_points_to_queue(void* args);

// Функция для тредов-потребителей
// Она считает многопоточно целевую функцию для сгенерированных tms-сетью точек
// Она берет точки из readQueue, когда очередь кончится она readQueue и writeQueue свапает и сигналит треду-поставщику поставлять
// Она сохраняет в std::set лучшие точки (их количество - nBestPoints)
void* calc_f_with_threads(void* args);

// Аргументы для треда-поставщика передаются через структуру
struct producerArgs {
    // Shared variables
    std::queue<Vector>& readQueue;
    std::queue<Vector>& writeQueue;
    pthread_mutex_t& consumerMutex;
    pthread_mutex_t& producerMutex;
    pthread_cond_t& canProduce;
    pthread_cond_t& canConsume;
    bool& endOfGeneration;
    bool& producerCanSignal;

    // Own variables
    const Vector& min;
    const Vector& max;
    const uint32_t dim;
    const uint32_t nAllPoints;
    const uint32_t miniBatchSize;
    const uint32_t maxQueueSize;
};

// Аргументы для тредов-потребителей передаются через структуру
struct consumerArgs {
    // Shared variables
    std::queue<Vector>& readQueue;
    std::queue<Vector>& writeQueue;
    pthread_mutex_t& consumerMutex;
    pthread_mutex_t& producerMutex;
    pthread_cond_t& canProduce;
    pthread_cond_t& canConsume;
    bool& endOfGeneration;
    bool& producerCanSignal;

    pthread_mutex_t& writeMutex;
    std::set<std::pair<Real, Vector>>& candidates;

    // Own variables
    const Function f;
    const uint32_t nBestPoints;
};


struct GlobalMinimizationResults {
    std::vector<std::vector<IterationData> > AllResults;
    std::vector<IterationData> BestMethodForEachPoint;
    std::vector<IterationData> BestPointForEachMethod;
    IterationData GlobalMinimum;
};
// Главная функция, которая ищет глобальный минимум
// Она сама вызовет все функции выше
// Она возвращает вектор пар <целевая функция, точка>
// Размер вектора - nBestPoints
GlobalMinimizationResults
find_absmin(Function f, const StopCondition& stop_condition, uint32_t dim, uint32_t nBestPoints, uint32_t nAllPoints, uint32_t miniBatchSize, uint32_t maxQueueSize, const Vector& min, const Vector& max,uint32_t HowManyConsumers, bool checkBorders);
