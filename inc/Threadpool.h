#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

template<typename Task>
class Threadpool {
public:
    Threadpool(unsigned int thread_number, unsigned int max_task_number);
    ~Threadpool();
    bool append(Task* task);
    bool stop();
private:
    static void worker(void* arg);
    void cycle();
private:
    std::queue<Task*> task_queue;
    std::vector<std::thread> threads;
    std::mutex mutex_task;
    std::condition_variable cond_task;
    unsigned int thread_number;
    unsigned int task_number;
    unsigned int max_task_number;
    bool running;
    unsigned int stopped_thread;
};

template<typename Task>
Threadpool<Task>::Threadpool(unsigned int thread_number, unsigned int max_task_number) {
    this->thread_number = thread_number;
    this->max_task_number = max_task_number;
    this->task_number = 0;
    this->threads.resize(thread_number);
    this->running = true;
    for (unsigned int i = 0; i < thread_number; i++) {
        threads[i] = std::thread(Threadpool::worker, this);
        threads[i].detach();
    }
}

template<typename Task>
Threadpool<Task>::~Threadpool() {}

template<typename Task>
bool Threadpool<Task>::append(Task* task) {
    std::unique_lock<std::mutex> locker(mutex_task);
    if (task_number >= max_task_number) {
        locker.unlock();
        return false;
    }
    task_number++;
    task_queue.push(task);
    locker.unlock();
    cond_task.notify_one();
    return true;
}

template<typename Task>
void Threadpool<Task>::worker(void* arg) {
    Threadpool* tp = (Threadpool*)arg;
    tp->cycle();
}

template<typename Task>
void Threadpool<Task>::cycle() {
    while (running) {
        std::unique_lock<std::mutex> locker(mutex_task);
        cond_task.wait(locker, [&]() {return !task_queue.empty();});
        task_number--;
        Task* task = task_queue.front();
        task_queue.pop();
        locker.unlock();
        if (task == nullptr) {
            continue;
        }
        task->process();
    }
    std::unique_lock<std::mutex> locker(mutex_task);
    stopped_thread++;
    locker.unlock();
}

template<typename Task>
bool Threadpool<Task>::stop() {
    stopped_thread = 0;
    running = false;
    while (stopped_thread < thread_number) {
        sleep(1);
    }
    return true;
}


#endif