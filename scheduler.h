#ifndef __sometuke__scheduler__
#define __sometuke__scheduler__

#include "sometuke/types.h"

#include <memory>
#include <list>
#include <map>
#include <cassert>

namespace sometuke {
using namespace std;

class Timer : public enable_shared_from_this<Timer> {
public:
    static const unsigned int REPEAT_FOREVER;

    Timer(const ii_time interval = 0,
          const unsigned int repeat = REPEAT_FOREVER,
          const ii_time delay = 0)
        : paused_(false) {
        set_interval(interval);
        set_repeat(repeat);
        set_delay(delay);
    }

    virtual ~Timer() {}

    virtual bool Tick(const ii_time delta_time);
    virtual bool Update(const ii_time delta) = 0;

    void set_interval(const ii_time interval) {
        interval_ = interval;
        elapsed_  = 0;
    }

    void set_repeat(const unsigned int repeat) {
        repeat_ = repeat;
        run_forever_ = (repeat == REPEAT_FOREVER);
        num_executed_ = 0;
    }

    void set_delay(const ii_time delay) {
        delay_ = delay;
        use_delay_ = (delay > 0);
    }

    const bool paused() const { return paused_; }
    void Pause()  { paused_ = true; }
    void Resume() { paused_ = false; }

    void Schedule();
    void Schedule(const ii_time interval,
                  const unsigned int repeat = REPEAT_FOREVER,
                  const ii_time delay = 0) {
        set_interval(interval);
        set_repeat(repeat);
        set_delay(delay);
        Schedule();
    }
    
    void UnSchedule();

private:
    bool paused_;
    ii_time elapsed_;
    ii_time interval_;
    ii_time delay_;
    bool use_delay_;
    unsigned int repeat_;
    bool run_forever_;
    int num_executed_;
};

class ProcessManager;

template <class T>
class TimerDelegator : public Timer {
public:
    static shared_ptr<Timer> Create(T *handler,
                                    const ii_time interval = 0,
                                    const unsigned int repeat = Timer::REPEAT_FOREVER,
                                    const ii_time delay = 0) {
        shared_ptr<T> handler_ptr = static_pointer_cast<T>(handler->shared_from_this());
        return make_shared<TimerDelegator<T> >(handler_ptr, interval, repeat, delay);
    }

    TimerDelegator(weak_ptr<T> delegate,
                  const ii_time interval = 0,
                  const unsigned int repeat = Timer::REPEAT_FOREVER,
                  const ii_time delay = 0)
        : Timer(interval, repeat, delay),
          delegate_(delegate) {
    }

    virtual bool Update(const ii_time delta) {
        if (shared_ptr<T> delegate = delegate_.lock()) {
            return delegate->Update(delta);
        } else {
            return false;
        }
    }

private:    
    weak_ptr<T> delegate_;
};

typedef list<shared_ptr<Timer> > TimerList;

class Scheduler {
public:
    void Schedule(shared_ptr<Timer> timer) {
        timers_.push_back(timer);
    }

    void Unschedule(const shared_ptr<Timer>& timer) {
        timers_.remove(timer);
    }
    
    void Update(const ii_time delta_time) {
        for (TimerList::iterator i = timers_.begin(); i != timers_.end();) {
            shared_ptr<Timer> timer = (*i);
            
            if (!timer->Tick(delta_time)) {
                timers_.erase(i);
            } else {
                ++i;
            }
        }
    }

private:
    TimerList timers_;
};

}

#endif /* defined(__sometuke__scheduler__) */

