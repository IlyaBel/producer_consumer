#ifndef PRODUCER_CONSUMER_H
#define PRODUCER_CONSUMER_H

#include "one_shot_event.h"
#include <mutex>
#include <condition_variable>
#include <queue>

template <class T>
class LimitedQueue {
private:
    const size_t N;
    std::queue<T> queue_;
    mutable std::mutex queue_mx;
    std::condition_variable prod_cv;
    std::condition_variable cons_cv;

public:
    LimitedQueue(size_t N_0);

    void push(const T& value);

    T pop();

    auto size() const;
};

#endif // PRODUCER_CONSUMER_H

template<class T>
LimitedQueue<T>::LimitedQueue(size_t N_0) : N(N_0) {}

template<class T>
void LimitedQueue<T>::push(const T &value) {
    //{...}
    std::unique_lock<std::mutex> lck(queue_mx);
    prod_cv.wait(lck, [this]{return queue_.size() < N;});
    queue_.push(value);
    lck.unlock();
    cons_cv.notify_one();
}

template<class T>
T LimitedQueue<T>::pop() {
    std::unique_lock<std::mutex> lck(queue_mx);
    cons_cv.wait(lck, [this]{return !queue_.empty();});
    T tmp = queue_.front();
    queue_.pop();
    lck.unlock();
    prod_cv.notify_all();//notify_all is needed here!
    return tmp;
}

template<class T>
auto LimitedQueue<T>::size() const{
    std::lock_guard<std::mutex> lck(queue_mx);
    return queue_.size();
}

