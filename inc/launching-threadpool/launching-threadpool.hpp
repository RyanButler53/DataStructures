#ifndef LAUNCHING_THREAD_QUEUE_HPP_INCLUDED
#define LAUNCHING_THREAD_QUEUE_HPP_INCLUDED

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <concepts>
#include <atomic>
#include <future>
#include <condition_variable>
#include <functional>


/**
 * @brief Functions for a Thread queue that can enqueue tasks and launch them
 * all at once. Works for functions that return a value
 * 
 * @tparam R, return type of the task. 
 */
template <typename R>
class LaunchingThreadQueue
{
private:
    /* data */
    size_t numThreads_;
    size_t numJobs_;

    std::queue<std::packaged_task<R()>> queue_; 
    std::vector<std::thread> threads_;
    std::vector<std::future<R>> futures_;

    std::mutex launchMutex_; // protects the queue

    // thread that does the actual work
    void workerThread(size_t thread_i);

    unsigned int calcNumThreads(unsigned int numThreads);

    void setupThreads();

    void clear();

    bool emptyQueue();

public:
    LaunchingThreadQueue(size_t numThreads = 0);
    ~LaunchingThreadQueue();

    /**
     * @brief Submits function F to the queue. 
     * 
     * @param f 
     */
    void submit(std::function<R()> f);

    /**
     * @brief Runs all the tasks and returns the result in a vector
     * 
     * @return std::vector<R> 
     */
    std::vector<R> run();

};

#include "launching-threadpool-private.hpp"

#endif // LAUNCHING_THREAD_QUEUE_HPP_INCLUDE