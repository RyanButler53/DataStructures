#include "launching-threadpool.hpp"

template <typename R>
LaunchingThreadQueue<R>::LaunchingThreadQueue(size_t numThreads):
    numThreads_{calcNumThreads(numThreads)}, numJobs_{0}
{}

template <typename R>
void LaunchingThreadQueue<R>::setupThreads(){
    try
    {
        for(unsigned i = 0;i<numThreads_;++i) {
            threads_.push_back(
                std::thread(&LaunchingThreadQueue<R>::workerThread,this, i));
        }
    } catch (std::exception &e) {
        throw;
    }
}

template <typename R>
unsigned int LaunchingThreadQueue<R>::calcNumThreads(unsigned int numThreads){
    unsigned int hardwareMax = std::thread::hardware_concurrency();
    if (numThreads == 0){
        numThreads = hardwareMax;
    } 
    if (hardwareMax == 0) { // if less than 2 or error
        return 2;
    } else {
        return std::min(numThreads, hardwareMax);
    }
}

template <typename R>
void LaunchingThreadQueue<R>::clear(){
    for (size_t i = 0; i < numThreads_; ++i){
        if (threads_[i].joinable()){
            threads_[i].join();
        } 
    }
    threads_.clear();
    futures_.clear();
    numJobs_ = 0;
}

template <typename R>
LaunchingThreadQueue<R>::~LaunchingThreadQueue(){
    clear();
}

template <typename R>
bool LaunchingThreadQueue<R>::emptyQueue(){
    std::scoped_lock lk(launchMutex_);
    return queue_.empty();
}

template <typename R>
void LaunchingThreadQueue<R>::submit(std::function<R()> f){
    std::packaged_task<R()> task(f);
    std::future<R> future(task.get_future());
    futures_.push_back(std::move(future));
    // std::queue &q = queueVec_[numJobs_%numThreads_];
    // auto& q = queueVec_[numJobs_ % numThreads_];
    queue_.push(std::move(task));
    ++numJobs_;
}

template <typename R>
std::vector<R> LaunchingThreadQueue<R>::run(){
    setupThreads();
    std::vector<R> ret;
    for (std::future<R> &f : futures_)
    {
        ret.push_back(f.get());

    }
    clear();
    return ret;
}

template <typename R>
void LaunchingThreadQueue<R>::workerThread(size_t thread_i){
    while (!emptyQueue()){
        std::unique_lock lk(launchMutex_);
        std::packaged_task<R()> task = std::move(queue_.front());
        queue_.pop();
        lk.unlock();
        task();

    }
}