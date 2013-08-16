#ifndef __sometuke__handler__
#define __sometuke__handler__

#include "sometuke/scheduler.h"
#include "sometuke/event_dispatcher.h"
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
    virtual void Update(const s2_time delta) {}
    virtual void HandleEvent(const shared_ptr<Event>& event) {}

    virtual bool paused() const { return paused_; }
    virtual void Pause()  { paused_ = true; }
    virtual void Resume() { paused_ = false; }

    virtual bool sleeping() const { return sleeping_; }
    virtual void Sleep() { sleeping_ = true;}
    virtual void WakeUp() { sleeping_ = false;}

    EventListener& listener();
    Timer& timer();

private:    
    shared_ptr<EventListener> listener_;
    shared_ptr<Timer> timer_;

    bool paused_;
    bool sleeping_;
};

class TimerAdapter : public Timer {
public:
    TimerAdapter(weak_ptr<Handler> handler,
                 const s2_time interval = 0,
                 const unsigned int repeat = Timer::REPEAT_FOREVER,
                 const s2_time delay = 0)
        : Timer(interval, repeat, delay),
          handler_(handler) {
    }

    void Update(const s2_time delta) {
        if (const shared_ptr<Handler>& handler = handler_.lock()) {
            if (!handler->paused() && !handler->sleeping()) {
                handler->Update(delta);
            }
        }
    }

private:    
    weak_ptr<Handler> handler_;
};

class EventAdapter : public EventListener {
public:
    EventAdapter(weak_ptr<Handler> handler)
        : handler_(handler) {
    }

    virtual ~EventAdapter() {}

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
