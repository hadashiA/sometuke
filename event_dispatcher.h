#ifndef __kawaii__event_dispatcher__
#define __kawaii__event_dispatcher__

#include "hashed_string.h"
#include "types.h"

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

struct Event {
public:
    Event(const EventType t, unsigned int tid = 0) :
        type(t),
        target_id(tid) {
        timestamp = std::time(NULL);
    }

    EventType type;
    time_t timestamp;
    unsigned int target_id;
};

class EventListener {
public:
    virtual ~EventListener() {}
    virtual bool EventHandle(const Event& e) = 0;
};

class EventDispatcher {
public:
    typedef std::map<EventType, EventTypeMetadata> EventTypeTable;
    typedef std::multimap<EventType, weak_ptr<EventListener> > EventListenerTable;

    EventDispatcher() :
        queue_index_(0) {
    }

    bool On(const EventType& type, weak_ptr<EventListener> listener);
    bool Off(const EventType& type);
    bool Off(shared_ptr<EventListener> listener);
    void Trigger(const Event& event);
    bool Queue(const Event& event);
    bool Tick(const ii_time max_time);

    bool IsValidType(const EventType& type);
    bool IsListerningType(const EventType& type);

private:
    std::list<Event> queues_[2];
    int queue_index_;

    EventTypeTable types_;
    EventListenerTable listeners_;
};

}

#endif /* defined(__kawaii__event_dispacher__) */

