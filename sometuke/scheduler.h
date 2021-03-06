#ifndef __sometuke__scheduler__
#define __sometuke__scheduler__

#include "sometuke/types.h"

#include <memory>
#include <list>
#include <map>
#include <cassert>
#include <iostream>
#include <functional>

namespace sometuke {
using namespace std;

typedef std::function<bool(const s2_time)> UpdateCallback;

class Timer : public enable_shared_from_this<Timer> {
public:
    static const unsigned int REPEAT_FOREVER;

    Timer(const s2_time interval = 0,
          const unsigned int repeat = REPEAT_FOREVER,
          const s2_time delay = 0) {
        set_interval(interval);
        set_repeat(repeat);
        set_delay(delay);
    }

    virtual ~Timer() {}

    bool Tick(const s2_time delta_time);

    void set_interval(const s2_time interval) {
        interval_ = interval;
        elapsed_  = 0;
    }

    void set_repeat(const unsigned int repeat) {
        repeat_ = repeat;
        run_forever_ = (repeat == REPEAT_FOREVER);
        num_executed_ = 0;
    }

    void set_delay(const s2_time delay) {
        delay_ = delay;
        use_delay_ = (delay > 0);
    }

    void On(UpdateCallback callback);
    void Off();

private:
    s2_time elapsed_;
    s2_time interval_;
    s2_time delay_;
    bool use_delay_;
    unsigned int repeat_;
    bool run_forever_;
    int num_executed_;
    UpdateCallback callback_;
};

typedef list<weak_ptr<Timer> > TimerList;

class Scheduler {
public:
    void ScheduleUpdate(const weak_ptr<Timer>& timer) {
        timers_.push_back(timer);
    }

    void Unschedule(const weak_ptr<Timer>& timer);
    
    void Update(const s2_time delta_time);

private:
    TimerList timers_;
};

}

#endif /* defined(__sometuke__scheduler__) */

