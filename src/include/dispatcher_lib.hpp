#pragma once
#include "loguru.hpp"
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <queue>
#include <string>
#include <thread>
#include <tuple>
#include <variant>
#include <vector>

using json = nlohmann::json;
constexpr loguru::Verbosity LOGLEVELDISPATCHER = loguru::Verbosity_INFO;

class DispatchFunctionQueue
{

protected:
    std::string name_;
    std::mutex lock_;
    std::vector<std::thread> threads_;
    std::condition_variable cv_;
    bool quit_ = false;
    std::deque<std::function<void(void)>> objectVector_;

    void dispatch_thread_handler(void)
    {
        loguru::set_thread_name(("Dispatch_" + name_).c_str());

        std::unique_lock<std::mutex> lock(lock_);

        do
        {
            // Wait until we have data or a quit signal
            cv_.wait(lock, [this] { return (objectVector_.size() || quit_); });

            // after wait, we own the lock
            if (!quit_ && objectVector_.size())
            {
                auto fnQueue = std::move(objectVector_.front());
                objectVector_.pop_front();

                // unlock now that we're done messing with the queue
                lock.unlock();

                try
                {
                    fnQueue();
                }
                catch (const std::exception &e)
                {
                    LOG_F(ERROR, "%s", e.what());
                }


                lock.lock();
            }
        } while (!quit_);
    }


public:
    std::unique_ptr<std::function<void(void)>> overflowFunction;

    DispatchFunctionQueue(std::string name, size_t thread_cnt = 1) : name_{std::move(name)}, threads_(thread_cnt)
    {

        LOG_F(INFO, "Dispatch threads for (%s): %zu\n", name_.c_str(), thread_cnt);

        for (size_t i = 0; i < threads_.size(); i++)
        {
            threads_[i] = std::thread(&DispatchFunctionQueue::dispatch_thread_handler, this);
        }
    }
    DispatchFunctionQueue(){};

    ~DispatchFunctionQueue()
    {
        if (quit_)
            return;
        LOG_F(INFO, "Destructor for %s: Destroying dispatch threads...", name_.c_str());

        // Signal to dispatch threads that it's time to wrap up
        std::unique_lock<std::mutex> lock(lock_);
        quit_ = true;
        lock.unlock();
        cv_.notify_all();

        // Wait for threads to finish before we exit
        for (size_t i = 0; i < threads_.size(); i++)
        {
            if (threads_[i].joinable())
            {
                // LOG_F(INFO,"Destructor: Joining thread %zu until completion\n", i);
                threads_[i].join();
            }
        }
    }

    bool dispatch(std::function<void(void)> object, const size_t &queueSize = 0)
    {
        bool queueFull = false;

        std::unique_lock<std::mutex> lock(lock_);
        if (queueSize > 0 && objectVector_.size() >= queueSize)
        {
            auto dismissedObject = objectVector_.front();
            if (overflowFunction)
                (*overflowFunction)();
            //LOG_F(WARNING, "Queue is full, element is dismissed: %s", std::get<0>(dismissedObject).dump().c_str());

            objectVector_.pop_front();

            queueFull = true;
        }
        objectVector_.push_back(std::move(object));
        if (objectVector_.size() > 1)
            LOG_F(1, "Queue Size %s: %zu", name_.c_str(), objectVector_.size());

        // Manual unlocking is done before notifying, to avoid waking up
        // the waiting thread only to block again (see notify_one for details)
        lock.unlock();
        cv_.notify_one();
        return queueFull;
    }

    // Deleted operations
    DispatchFunctionQueue(const DispatchFunctionQueue &rhs) = delete;
    DispatchFunctionQueue &operator=(const DispatchFunctionQueue &rhs) = delete;
    DispatchFunctionQueue(DispatchFunctionQueue &&rhs) = delete;
    DispatchFunctionQueue &operator=(DispatchFunctionQueue &&rhs) = delete;
};

template <typename T>
class Dispatch_queue
{
protected:
    using objectMeta = std::tuple<T, json>;


    std::string name_;
    std::mutex lock_;
    std::vector<std::thread> threads_;
    std::condition_variable cv_;
    bool quit_ = false;
    std::deque<objectMeta> objectVector_;
    std::deque<objectMeta> prioObjectVector_;

    void dispatch_thread_handler(void);
    size_t MAXQUEUESIZE = 0;

    unsigned int looseCounter = 0;
    unsigned int counter_log = 0;
    unsigned int counter_shrink = 0;

    std::chrono::_V2::system_clock::time_point lastbatch;

    const int INTERVAL_LOGS = 100;

    const int INTERVAL_RESIZE = 1000;


    void logStatistics()
    {
        counter_log++;

        if (counter_log == INTERVAL_LOGS)
        {
            if (looseCounter > 0)
                LOG_F(ERROR, "Loose rate: %d", looseCounter);
            else
                VLOG_F(LOGLEVELDISPATCHER, "Loose rate: %d", looseCounter);

            VLOG_F(LOGLEVELDISPATCHER, "Queue size: %zu", getQueueSize());
            std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - lastbatch;
            VLOG_F(LOGLEVELDISPATCHER,
                   "Rate for batch %d: IN %f images/sec --> OUT %f",
                   INTERVAL_LOGS,
                   static_cast<double>(INTERVAL_LOGS) / elapsed.count(),
                   static_cast<double>(INTERVAL_LOGS - looseCounter) / elapsed.count());

            counter_log = 0;
            looseCounter = 0;
            lastbatch = std::chrono::high_resolution_clock::now();
        }
    }

    void shrinkVector()
    {
        counter_shrink++;

        if (counter_shrink == INTERVAL_RESIZE)
        {
            objectVector_.shrink_to_fit();
            prioObjectVector_.shrink_to_fit();

            counter_shrink = 0;
        }
    }
   


public:
    using fp_t = std::function<void(T, bool)>;
    using fp_t_parameter = std::function<void(T, json, bool)>;
    using fp_t_variant = std::variant<fp_t, fp_t_parameter>;
    std::vector<fp_t_variant> internalfunctions_;
    std::vector<fp_t_variant> internalfunctionsPrio_;

    std::unique_ptr<fp_t> overflowFunction;

    Dispatch_queue(std::string name, size_t thread_cnt = 1, size_t maxQueueSize = 0);
    Dispatch_queue();

    ~Dispatch_queue();

    bool dispatch(T object);
    bool dispatch(T object, json meta);
    bool dispatchPrio(T object);

    bool dispatchPrio(T object, json meta);


    size_t getQueueSize();
    // Deleted operations
    Dispatch_queue(const Dispatch_queue &rhs) = delete;
    Dispatch_queue &operator=(const Dispatch_queue &rhs) = delete;
    Dispatch_queue(Dispatch_queue &&rhs) = delete;
    Dispatch_queue &operator=(Dispatch_queue &&rhs) = delete;

    void setFunction(std::vector<fp_t> ops);
    void setFunction(std::vector<fp_t_variant> ops);

    void setFunction(fp_t ops);
    void setFunction(fp_t_parameter ops);

    void setFunctionPrio(fp_t ops);
    void setFunctionPrio(fp_t_parameter ops);

    void setOverflowFunction(fp_t &op);

    size_t getVectorSize();
    std::tuple<T, json> &getFront();
    std::deque<objectMeta> *getQueue();

    size_t clearQueue();

    private:
     void executeFuntions(T &dto, json &meta, const std::vector<fp_t_variant> &functions)
    {

        for (size_t i = 0; i < functions.size(); i++)
        {
            try
            {
                if (functions[i].index() == 0)
                {
                    fp_t imageFunction = std::get<fp_t>(functions[i]);
                    imageFunction(dto, getQueueSize() < MAXQUEUESIZE);
                }
                else
                {
                    fp_t_parameter imageFunction = std::get<fp_t_parameter>(functions[i]);
                    imageFunction(dto, meta, getQueueSize() < MAXQUEUESIZE);
                }
            }
            catch (const std::exception &e)
            {
                LOG_F(ERROR, "%s", e.what());
            }
        }
    }
};
template <typename T>
Dispatch_queue<T>::Dispatch_queue()
{
}

template <typename T>
Dispatch_queue<T>::Dispatch_queue(std::string name, size_t thread_cnt, size_t maxQueueSize)
    : name_{std::move(name)}, threads_(thread_cnt), MAXQUEUESIZE(maxQueueSize)
{

    LOG_F(INFO, "Dispatch threads for (%s): %zu\n", name_.c_str(), thread_cnt);

    lastbatch = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < threads_.size(); i++)
    {
        threads_[i] = std::thread(&Dispatch_queue::dispatch_thread_handler, this);
    }
}
template <typename T>
Dispatch_queue<T>::~Dispatch_queue()
{
    if (quit_)
        return;
    LOG_F(INFO, "Destructor for %s: Destroying dispatch threads...", name_.c_str());

    // Signal to dispatch threads that it's time to wrap up
    std::unique_lock<std::mutex> lock(lock_);
    quit_ = true;
    lock.unlock();
    cv_.notify_all();

    // Wait for threads to finish before we exit
    for (size_t i = 0; i < threads_.size(); i++)
    {
        if (threads_[i].joinable())
        {
            // LOG_F(INFO,"Destructor: Joining thread %zu until completion\n", i);
            threads_[i].join();
        }
    }
}
template <typename T>
bool Dispatch_queue<T>::dispatch(T object)
{
    bool queueFull = false;

    std::unique_lock<std::mutex> lock(lock_);
    if (MAXQUEUESIZE > 0 && objectVector_.size() >= MAXQUEUESIZE)
    {
        objectMeta dismissedObject = objectVector_.front();
        if (overflowFunction)
            (*overflowFunction)(std::get<0>(dismissedObject), false);

        objectVector_.pop_front();

        queueFull = true;
    }
    objectVector_.push_back(objectMeta(std::move(object), json()));
    if (objectVector_.size() > 1)
        LOG_F(INFO, "Queue Size %s: %zu", name_.c_str(), objectVector_.size());


    logStatistics();

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    cv_.notify_one();
    return queueFull;
}
template <typename T>
bool Dispatch_queue<T>::dispatch(T object, json meta)
{
    bool queueFull = false;
    std::unique_lock<std::mutex> lock(lock_);
    if (MAXQUEUESIZE > 0 && objectVector_.size() >= MAXQUEUESIZE)
    {
        objectMeta dismissedObject = objectVector_.front();
        if (overflowFunction)
            (*overflowFunction)(std::get<0>(dismissedObject), false);
        objectVector_.pop_front();
        queueFull = true;
    }
    objectVector_.push_back(objectMeta(std::move(object), meta));

    logStatistics();


    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    cv_.notify_one();
    return queueFull;
}
template <typename T>
bool Dispatch_queue<T>::dispatchPrio(T object)
{

    std::unique_lock<std::mutex> lock(lock_);
    bool queueFull = (MAXQUEUESIZE > 0 && prioObjectVector_.size() >= MAXQUEUESIZE);

    prioObjectVector_.push_back(objectMeta(std::move(object), json()));
    if (prioObjectVector_.size() > 1)
        LOG_F(INFO, "Queue Size %s: %zu", name_.c_str(), prioObjectVector_.size());


    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    cv_.notify_one();
    return queueFull;
}
template <typename T>
bool Dispatch_queue<T>::dispatchPrio(T object, json meta)
{
    std::unique_lock<std::mutex> lock(lock_);
    bool queueFull = (MAXQUEUESIZE > 0 && prioObjectVector_.size() >= MAXQUEUESIZE);

    prioObjectVector_.push_back(objectMeta(std::move(object), meta));


    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    cv_.notify_one();
    return queueFull;
}
template <typename T>
void Dispatch_queue<T>::dispatch_thread_handler(void)
{
    std::unique_lock<std::mutex> lock(lock_);
    loguru::set_thread_name(("Dispatch_" + name_).c_str());

    do
    {
        // Wait until we have data or a quit signal
        cv_.wait(lock, [this] { return (objectVector_.size() || prioObjectVector_.size() || quit_); });

        // after wait, we own the lock
        if (!quit_)
        {
            if (prioObjectVector_.size())
            {
                auto [dto, meta] = std::move(prioObjectVector_.front());
                prioObjectVector_.pop_front();
                shrinkVector();
                lock.unlock();
                executeFuntions(dto, meta, internalfunctionsPrio_);
            }
            else if (objectVector_.size())
            {
                auto [dto, meta] = std::move(objectVector_.front());
                objectVector_.pop_front();

                shrinkVector();
                lock.unlock();
                executeFuntions(dto, meta, internalfunctions_);
            }
            else
            {
                lock.unlock();
            }


            lock.lock();
        }
    } while (!quit_);
}
template <typename T>
size_t Dispatch_queue<T>::getQueueSize()
{
    return objectVector_.size();
}

template <typename T>
void Dispatch_queue<T>::setFunction(std::vector<fp_t> ops)
{
    for (size_t i = 0; i < ops.size(); i++)
    {
        std::variant<fp_t, fp_t_parameter> functionCall;
        functionCall = ops[i];
        internalfunctions_.push_back(functionCall);
    }
}


template <typename T>
void Dispatch_queue<T>::setFunction(std::vector<fp_t_variant> ops)
{
    for (size_t i = 0; i < ops.size(); i++)
    {
        std::variant<fp_t, fp_t_parameter> functionCall;
        functionCall = ops[i];
        internalfunctions_.push_back(functionCall);
    }
}

template <typename T>
void Dispatch_queue<T>::setFunction(fp_t op)
{
    std::variant<fp_t, fp_t_parameter> functionCall;
    functionCall = op;

    internalfunctions_.push_back(functionCall);
}

template <typename T>
void Dispatch_queue<T>::setFunction(fp_t_parameter op)
{
    std::variant<fp_t, fp_t_parameter> functionCall;
    functionCall = op;

    internalfunctions_.push_back(std::move(functionCall));
}

template <typename T>
void Dispatch_queue<T>::setFunctionPrio(fp_t op)
{
    std::variant<fp_t, fp_t_parameter> functionCall;
    functionCall = op;

    internalfunctionsPrio_.push_back(functionCall);
}

template <typename T>
void Dispatch_queue<T>::setFunctionPrio(fp_t_parameter op)
{
    std::variant<fp_t, fp_t_parameter> functionCall;
    functionCall = op;

    internalfunctionsPrio_.push_back(std::move(functionCall));
}



template <typename T>
void Dispatch_queue<T>::setOverflowFunction(fp_t &op)
{
    overflowFunction = std::make_unique<fp_t>(op);
}


template <typename T>
size_t Dispatch_queue<T>::getVectorSize()
{
    return objectVector_.size();
}

template <typename T>
std::tuple<T, json> &Dispatch_queue<T>::getFront()
{
    return objectVector_.front();
}

template <typename T>
std::deque<std::tuple<T, json>> *Dispatch_queue<T>::getQueue()
{
    return &objectVector_;
}

template <typename T>
size_t Dispatch_queue<T>::clearQueue()
{
    std::unique_lock<std::mutex> lock(lock_);
    size_t queueSize = objectVector_.size();
    objectVector_.clear();
    return queueSize;
}


using DispatchQueueImages = Dispatch_queue<cv::Mat>;
