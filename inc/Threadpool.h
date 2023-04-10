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
    void append(Task* task);
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
void Threadpool<Task>::append(Task* task) {

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
        Task *task = task_queue.front();
        task_queue.pop();
        locker.unlock();
        task->process();
    }
}


#endif