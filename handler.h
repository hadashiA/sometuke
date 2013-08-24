#ifndef __sometuke__handler__
#define __sometuke__handler__

#include "sometuke/director.h"
#include "sometuke/memory_pool.h"

namespace sometuke {

class Handler : public TimerInterface,
                public EventListenerInterface,
      		public enable_shared_from_this<Handler> {
public:
    Handler()
        : paused_(false),
          sleeping_(false) {
    }

    virtual ~Handler() {}
    virtual bool Update(const s2_time delta) { return true; }
    virtual void HandleEvent(const shared_ptr<Event>& event) {}

    virtual bool paused() const { return paused_; }
    virtual void Pause()  { paused_ = true; }
    virtual void Resume() { paused_ = false; }

    virtual bool sleeping() const { return sleeping_; }
    virtual void Sleep() { sleeping_ = true;}
    virtual void WakeUp() { sleeping_ = false;}

    template <typename T>
    bool ListenTo() {
        return listener().ListenTo<T>();
    }

    bool ListenTo(const EventType& type) {
        return listener().ListenTo(type);
    }

    template<class T>
    bool Queue() {
        return Director::Instance().dispatcher().Queue<T>();
    }

    template<class T, class Arg1, class... Args>
    bool Queue(Arg1&& arg1, Args&& ... args) {
        return Director::Instance().dispatcher().Queue<T, Arg1, Args...>(std::forward<Arg1>(arg1),
                                 std::forward<Args>(args)...);
    }

    bool StopListering() {
        return listener().StopListering();
    }

    void ScheduleUpdate() {
        timer().ScheduleUpdate();
    }

    void ScheduleUpdate(const s2_time interval,
                        const unsigned int repeat = Timer::REPEAT_FOREVER,
                        const s2_time delay = 0) {
        timer().ScheduleUpdate(interval, repeat, delay);
    }

    void UnSchedule() {
        timer().UnSchedule();
    }

protected:
    EventListener& listener();
    Timer& timer();

private:    
    shared_ptr<EventListener> listener_;
    shared_ptr<Timer> timer_;

    bool paused_;
    bool sleeping_;
};

class TimerProxy : public Timer {
public:
    TimerProxy(weak_ptr<Handler> handler,
               const s2_time interval = 0,
               const unsigned int repeat = Timer::REPEAT_FOREVER,
               const s2_time delay = 0)
        : Timer(interval, repeat, delay),
          handler_(handler) {
    }

    bool Update(const s2_time delta) {
        if (const shared_ptr<Handler>& handler = handler_.lock()) {
            return (!handler->paused() && !handler->sleeping() && handler->Update(delta));
        }
        return false;
    }

private:    
    weak_ptr<Handler> handler_;
};

class EventProxy : public EventListener {
public:
    EventProxy(weak_ptr<Handler> handler)
        : handler_(handler) {
    }

    void HandleEvent(const shared_ptr<Event>& e) {
        if (const shared_ptr<Handler>& handler = handler_.lock()) {
            if (!handler->paused() && !handler->sleeping()) {
                handler->HandleEvent(e);
            }
        }
    }

private:
    weak_ptr<Handler> handler_;
};

}

#endif /* defined(__sometuke__handler__) */
