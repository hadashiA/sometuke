#ifndef __sometuke__scheduler__
#define __sometuke__scheduler__

#include "sometuke/types.h"

#include <memory>
#include <list>
#include <map>
#include <cassert>
#include <iostream>

namespace sometuke {
using namespace std;

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

    virtual bool Update(const s2_time delta) = 0;
    virtual const bool paused() const = 0;

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

    void Schedule();
    void Schedule(const s2_time interval,
                  const unsigned int repeat = REPEAT_FOREVER,
                  const s2_time delay = 0) {
        set_interval(interval);
        set_repeat(repeat);
        set_delay(delay);
        Schedule();
    }
    
    void UnSchedule();

private:
    s2_time elapsed_;
    s2_time interval_;
    s2_time delay_;
    bool use_delay_;
    unsigned int repeat_;
    bool run_forever_;
    int num_executed_;
};

class ProcessManager;

template <class T>
class TimerAdapter : public Timer {
public:
    static shared_ptr<Timer> Create(T *handler,
                                    const s2_time interval = 0,
                                    const unsigned int repeat = Timer::REPEAT_FOREVER,
                                    const s2_time delay = 0) {
        shared_ptr<T> handler_ptr = static_pointer_cast<T>(handler->shared_from_this());
        return make_shared<TimerAdapter<T> >(handler_ptr, interval, repeat, delay);
    }

    TimerAdapter(weak_ptr<T> delegate,
                 const s2_time interval = 0,
                 const unsigned int repeat = Timer::REPEAT_FOREVER,
                 const s2_time delay = 0)
        : Timer(interval, repeat, delay),
          delegate_(delegate) {
    }

    virtual bool Update(const s2_time delta) {
        if (shared_ptr<T> delegate = delegate_.lock()) {
            return delegate->Update(delta);
        } else {
            return false;
        }
    }

    virtual const bool paused() const {
        if (shared_ptr<T> delegate = delegate_.lock()) {
            return delegate->paused();
        } else {
            return false;
        }
    }
    

private:    
    weak_ptr<T> delegate_;
};

typedef list<weak_ptr<Timer> > TimerList;

class Scheduler {
public:
    void Schedule(const weak_ptr<Timer>& timer) {
        timers_.push_back(timer);
    }

    void Unschedule(const weak_ptr<Timer>& timer);
    
    void Update(const s2_time delta_time);

    double Now();

private:
    TimerList timers_;
};

}

#endif /* defined(__sometuke__scheduler__) */

