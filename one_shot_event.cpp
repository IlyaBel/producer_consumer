#include "one_shot_event.h"

void one_shot_event::wait()
{
    std::unique_lock<std::mutex> lock(mx_);
    cv_.wait(lock, [this]() { return is_signalled_; });
}

void one_shot_event::signal()
{
    {
        std::lock_guard<std::mutex> lock(mx_);
        is_signalled_ = true;
    }

    cv_.notify_all();
}
