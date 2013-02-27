#ifndef __kawaii__event_dispatcher__
#define __kawaii__event_dispatcher__

#include "hashed_string.h"
#include "types.h"
#include "memory_pool.h"

#include <memory>
#include <list>
#include <map>
#include <ctime>

namespace kawaii {
using namespace std;

typedef HashedString EventType;

typedef enum {
    kScriptDefined,             // Event is defined in script.
    kCodeEventOnly,             // Event is defined by main code, and is NOT callable from script.
    kCodeEventScriptCallable,   // Event is defined by code, but is callable from script.
} EventCallable;

struct EventTypeMetadata {
    explicit EventTypeMetadata(EventCallable c) : callable(c) {}
    EventCallable callable;
};

struct Event : public GeneralPoolable {
public:
    Event(const EventType& t) : type(t) {
        timestamp = std::time(NULL);
    }

    ~Event() {}

    EventType type;
    time_t timestamp;
};

class EventListener : public enable_shared_from_this<EventListener> {
public:
    virtual ~EventListener() {}
    virtual bool HandleEvent(shared_ptr<Event> e) = 0;

    bool On(const EventType& type);
    bool Off(const EventType& type);
    bool Off();

    template <typename E>
    bool On() {
        return On(E::TYPE);
    }

    template <typename E>
    bool Off() {
        return Off(E::TYPE);
    }
};

class EventDispatcher {
public:
    // typedef std::map<EventType, EventTypeMetadata> EventTypeTable;
    typedef std::multimap<EventType, weak_ptr<EventListener> > EventListenerTable;

    EventDispatcher() :
        active_queue_index_(0) {
    }

    bool On(const EventType& type, weak_ptr<EventListener> listener);
    bool Off(const EventType& type, shared_ptr<EventListener> listener);
    bool Off(const EventType& type);
    bool Off(shared_ptr<EventListener> listener);

    template <typename E>
    bool On(weak_ptr<EventListener> listener) {
        return On(E::TYPE, listener);
    }

    template <typename E>
    bool Off() {
        return Off(E::TYPE);
    }

    template <typename E>
    bool Off(weak_ptr<EventListener> listener) {
        return Off(E::TYPE, listener);
    }

    bool Trigger(shared_ptr<Event> event);
    bool Queue(shared_ptr<Event> event);
    bool Tick(const ii_time max_time);

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
};

}

#endif /* defined(__kawaii__event_dispacher__) */

