#ifndef __sometuke__event_dispatcher__
#define __sometuke__event_dispatcher__

#include "sometuke/types.h"
#include "sometuke/memory_pool.h"
#include "sometuke/hashed_string.h"

#include <memory>
#include <list>
#include <map>
#include <functional>

#include <cmath>
#include <chrono>

namespace sometuke {
using namespace std;

typedef HashedString EventType;

struct Event {
    Event(const char *str)
        : Event(EventType(str)) {
    }
    
    Event(const EventType& t)
        : type(t) {
        created_at = chrono::system_clock::now();
    }

    virtual ~Event() {}

    EventType type;
    chrono::time_point<chrono::system_clock> created_at;
};
    
class EventListener;
    
typedef std::function<void(const shared_ptr<Event>&)> EventCallback;
    
struct EventHandler {
    EventCallback callback;
    weak_ptr<EventListener> listener;
};

class EventListenerInterface {
public:
    virtual ~EventListenerInterface() {}
    virtual void HandleEvent(const shared_ptr<Event>& event) = 0;
};

class EventListener : public EventListenerInterface,
                      public enable_shared_from_this<EventListener> {
public:
    virtual ~EventListener() {}

    bool ListenTo(const EventType& type);
    bool StopListering();

    template <typename E>
    bool ListenTo() {
        return ListenTo(E::TYPE);
    }
    
    void HandleEvent(const shared_ptr<Event>& event) {
        // dummy
    }

    template <typename E>
    void On(EventCallback callback) {
        On(E::TYPE, callback);
    }

    void On(const EventType& e, EventCallback handler);
};

class EventDispatcher {
public:
    typedef multimap<EventType, weak_ptr<EventListener> > EventListenerTable;

    EventDispatcher() :
        active_queue_index_(0) {
    }

    bool On(const EventType& type, shared_ptr<EventListener> listener);
    bool Off(const EventType& type, shared_ptr<EventListener> listener);
    bool Off(const EventType& type);
    bool Off(shared_ptr<EventListener> listener);

    template <typename E>
    bool On(shared_ptr<EventListener> listener) {
        return On(E::TYPE, listener);
    }

    void On(const EventType& type, EventHandler handler);

    template <typename E>
    bool Off() {
        return Off(E::TYPE);
    }

    template <typename E>
    bool Off(shared_ptr<EventListener> listener) {
        return Off(E::TYPE, listener);
    }

    bool Trigger(const shared_ptr<Event>& event);

    template <class T>
    bool Trigger() {
        shared_ptr<T> event(new T);
        return Trigger(event);
    }

    template <class T, class Arg1, class... Args>
    bool Trigger(Arg1&& arg1, Args&& ... args) {
        shared_ptr<T> event(new T(std::forward<Arg1>(arg1), std::forward<Args>(args)...));
        return Trigger(event);
    }

    bool Queue(const shared_ptr<Event>& event);

    template<class T>
    bool Queue() {
        shared_ptr<T> event = Pool<T>();
        return Queue(event);
    }

    template<class T, class Arg1, class... Args>
    bool Queue(Arg1&& arg1, Args&& ... args) {
        shared_ptr<T> event(Pool<T>(std::forward<Arg1>(arg1), std::forward<Args>(args)...));
        return Queue(event);
    }

    bool Tick(const s2_time max_time);

private:
    bool IsValidType(const EventType& type) const;
    bool IsListerningType(const EventType& type) const;

    enum {
        NUM_QUEUES = 2
    };

    list<shared_ptr<Event> > queues_[NUM_QUEUES];
    int active_queue_index_;

    // EventTypeTable types_;
    EventListenerTable listeners_;

    multimap<EventType, EventHandler> handlers_;
};

}

#endif /* defined(__sometuke__event_dispacher__) */