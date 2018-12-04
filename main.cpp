#include "limited_queue.h"
#include <thread>
#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

template <class T>
void prod_work(LimitedQueue<T> &queue, one_shot_event& event, size_t tries){

    event.wait();

    for (size_t i = 0; i < tries; i++){
        queue.push(10);
    }
}

template <class T>
void cons_work(LimitedQueue<T> &queue, one_shot_event& event, size_t tries){

    event.wait();

    for (size_t i = 0; i < tries; i++){
        queue.pop();
    }
}

int main ()
{
    using data_t = int;
    constexpr size_t queue_size = 5;
    constexpr size_t tries = 1000;
    constexpr size_t threads_number = 400;

    assert(threads_number%2 == 0);

    LimitedQueue<data_t> queue(queue_size);
    one_shot_event event;

    std::thread prods[threads_number/2];
    std::thread cons[threads_number/2];

    for (size_t i=0; i<threads_number/2; ++i)
        cons[i] = std::thread([&queue, &event, &tries]{cons_work(queue, event, tries);});

    for (size_t i=0; i<threads_number/2; ++i)
        prods[i] = std::thread([&queue, &event, &tries]{prod_work(queue, event, tries);});

    cout << threads_number/2 << " producers and " <<
        threads_number/2 << " are ready to go.\n";
    cout << "Press any key to start\n";

    getchar();

    event.signal();

    for (auto& th : prods) th.join();
    for (auto& th : cons) th.join();

    cout << "Queue size after work: " << queue.size() << endl;

    return 0;
}
