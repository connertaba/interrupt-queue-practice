#pragma once
#include <queue> //stores pending events
#include <mutex> //protects memory
#include <condition_variable> //allows the threads to sleep

template<typename T>

class Queue {
    private:
        std::queue<T> queue_; //contains the pending events
        std::mutex mutex_; //keeps threds from cutting each other in line
        std::condition_variable cv_; //tells the main loop to not do anything until something is in the queue

    public:
        //adds something to the queue and tells the main loop to do smth
        void push(const T& value) {
            {
                std::lock_guard<std::mutex> lock(mutex_); //keeps everything in line
                queue_.push(value);//adds something to the back of the queue
            }

            cv_.notify_one(); //tells the main loop to do smth
        };

        //waits until a notification is recived and then moves along in the queue
        void wait_and_pop(T& value) {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this] {  //waits for a notification to do this
                return !queue_.empty(); //accesses the the local queue_ variable and returns if it's empty or not
                }
            );
        

        value = queue_.front(); //sets the value to whats at the fron of the queue
        queue_.pop(); //deletes the value at the front of the queue
        }

};