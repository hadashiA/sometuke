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

typedef unsigned int client_id;
typedef HashedString EventType;

struct Event {
    Event(const char *str)
        : Event(EventType(str)) {
    }
    
    Event(const EventType& t)
        : type(t),
          sender_id(0),
          quick(false) {
        created_at = chrono::system_clock::now();
    }

    virtual ~Event() {}

    EventType type;
    client_id sender_id;
    bool quick;
    chrono::time_point<chrono::system_clock> created_at;
};
    
typedef std::function<void(const shared_ptr<Event>&)> EventCallback;
    
class EventEmitter {
public:
    static client_id GenerateClientId() {
        static client_id __last_id = 0;
        return ++__last_id;
    }

    EventEmitter()
        : cid_(GenerateClientId()) {
    }

    ~EventEmitter() {
        Off();
    }

    template <typename E>
    void On(const client_id listener_id, EventCallback callback) {
        On(E::TYPE, listener_id, callback);
    }

    void On(const EventType& e,
            const client_id sender_id,
            const EventCallback& callback);

    void Off();

    void Trigger(const shared_ptr<Event>& event);

    template<class T>
    void Trigger() {
        shared_ptr<T> event = Pool<T>();
        return Trigger(event);
    }

    template<class T, class Arg1, class... Args>
    void Trigger(Arg1&& arg1, Args&& ... args) {
        shared_ptr<T> event(Pool<T>(std::forward<Arg1>(arg1), std::forward<Args>(args)...));
        return Trigger(event);
    }

private:
    const client_id cid_;
};

struct EventHandler {
    EventHandler(const client_id sender_id,
                 const client_id receiver_id,
                 const EventCallback& callback)
        : sender_id(sender_id),
          receiver_id(receiver_id),
          callback(callback) {
    client_id sender_id;
    client_id receiver_id;
    EventCallback callback;
};

class EventDispatcher {
public:
    typedef std::multimap<EventType, EventHandler> handlers_;

    EventDispatcher() :
        active_queue_index_(0) {
    }

    void On(const EventType& type,
            const client_id sender_id,
            const client_id receiver_id,
            const EventCallback& callback);
    void Off(const client_id receiver_id);
    void Trigger(const shared_ptr<Event>& event)
    bool Tick(const s2_time max_time);

private:
    bool IsValidType(const EventType& type) const;
    bool IsListerningType(const EventType& type) const;

    enum {
        NUM_QUEUES = 2
    };

    std::list<shared_ptr<Event> > queues_[NUM_QUEUES];
    size_t active_queue_index_;

    EventListenerTable listeners_;
};

}

#endif /* defined(__sometuke__event_dispacher__) */
