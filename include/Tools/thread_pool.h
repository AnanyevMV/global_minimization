#pragma once

#include <queue>
#include <functional>
#include <pthread.h>

class ThreadPool {
public:
    explicit ThreadPool(size_t);

    void CreateThreads();

    bool Add(const std::function<void()>& func);
    void WaitForAll();

    static void* WorkerThreadProc(void* object);
    static uint32_t GetHardwareCoresAmount();

private:
    std::vector<pthread_t> workers_;
    std::queue<std::function<void()>> tasks_;
    pthread_mutex_t queue_mutex_{};
    //pthread_cond_t queue_cond_;
    size_t threads_amount_;
};
