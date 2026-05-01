
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstdlib>
#include <ctime>
#include "queue.h"

//simulates a packet from hardware
struct Event {
    int id; //id of the packet
    std::chrono::steady_clock::time_point timestamp; //time packet was created
};

std::atomic<bool> running(true); //creates a variable that can be accessed by multiple threads and is set to true 

void interrupt_thread(Queue<Event>& q) { //models a system interrupt
    int event_id = 0; //creates a variable to keep track of the event id
    while (running) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(rand() %101)
        ); //sleeps for a random amount of time between 0 and 1000 milliseconds

        Event e; //creates a new event
        e.id = event_id++; //set the event id and increment it for the next event
        e.timestamp = std::chrono::steady_clock::now(); //set the timestamp to the current time

        q.push(e); //adds the event to the queue


    }
};

int main() {
    auto program_start = std::chrono::steady_clock::now(); //gets the time the program started
    srand(time(0)); //seeds the random number generator with the current time
    Queue<Event> event_queue; //creates a queue to hold the events
    std::thread isr( //creates a thread called isr and gives it a function to run, and that function a reference to the event queue
        interrupt_thread,
        std::ref(event_queue)
    ); 

    while (true) {
        Event e;
        event_queue.wait_and_pop(e); //waits for an event to be added to the queue and then pops it from the queue
        auto now = std::chrono::steady_clock::now(); //gets the current time
        auto latency = //finds the latency of the event in microseconds by subtracting the event timestamp from the current time and converting it to microseconds
            std::chrono::duration_cast<std::chrono::microseconds>(
                now - e.timestamp
            ).count();

        std::cout << "Event " << e.id
                  << " | latency: " << latency << " us\n"; //prints the event id and the latency in milliseconds
   
        
        auto elapsed = // calculated how long the program has been running in seconds
            std::chrono::duration_cast<std::chrono::seconds>(
                now - program_start).count();

        if (elapsed >= 10){ //if the program has been running for 10 seconds or more, exit the loop
            break;
        }

    }

    running = false; //sets running to false to stop the interrupt thread
    isr.join(); //safely lets the thread finish then exit

    return 0; //exits the program

};