#include "Tools/thread_pool.h"

#include <thread>
#include <algorithm>

ThreadPool::ThreadPool(size_t threads)
    : threads_amount_(threads)
{
    pthread_mutex_init(&queue_mutex_, nullptr);
}

void ThreadPool::CreateThreads()
{
    for (size_t i = 0; i < threads_amount_; ++i)
    {
        pthread_t thr;
        pthread_create(&thr, nullptr, &ThreadPool::WorkerThreadProc, static_cast<void*>(this));

        workers_.push_back(thr);
    }
}

void* ThreadPool::WorkerThreadProc(void* object)
{
    ThreadPool* thread_pool = static_cast<ThreadPool*>(object);

    for (;;)
    {
        pthread_mutex_lock(&thread_pool->queue_mutex_);

        if (thread_pool->tasks_.empty())
        {
            pthread_mutex_unlock(&thread_pool->queue_mutex_);
            break;
        }

        std::function<void()> task = std::move(thread_pool->tasks_.front());

        thread_pool->tasks_.pop();

        pthread_mutex_unlock(&thread_pool->queue_mutex_);

        task();
    }

    pthread_exit(nullptr);

    return nullptr;
}

bool ThreadPool::Add(const std::function<void()>& func)
{
    pthread_mutex_lock(&queue_mutex_);

    tasks_.push(func);

    pthread_mutex_unlock(&queue_mutex_);

    return true;
}

void ThreadPool::WaitForAll()
{
    for (pthread_t worker : workers_)
        pthread_join(worker, nullptr);
}

uint32_t ThreadPool::GetHardwareCoresAmount()
{
    const uint32_t cores = std::max(1u, std::thread::hardware_concurrency());
    return cores > 4 ? cores - 1 : cores;
}
