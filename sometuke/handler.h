#ifndef __sometuke__handler__
#define __sometuke__handler__

#include "sometuke/director.h"
#include "sometuke/memory_pool.h"

namespace sometuke {

class Handler : public EventListenerInterface,
      		public enable_shared_from_this<Handler> {
public:
    Handler()
        : paused_(false),
          sleeping_(false) {
    }

    virtual ~Handler() {}
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

protected:
    EventListener& listener();

private:    
    shared_ptr<EventListener> listener_old_;

    bool paused_;
    bool sleeping_;
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
