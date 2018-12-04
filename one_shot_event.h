#ifndef ONE_SHOT_EVENT_H
#define ONE_SHOT_EVENT_H

#include <mutex>
#include <condition_variable>

class one_shot_event
{
 bool is_signalled_ = false;
 std::condition_variable cv_;
 std::mutex mx_;

public:
 void wait();

 void signal();
};

#endif // ONE_SHOT_EVENT_H
