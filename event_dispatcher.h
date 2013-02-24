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

class EventListener : public enable_shared_from_this<EventListener> {
public:
    virtual ~EventListener() {}
    virtual bool HandleEvent(shared_ptr<Event> e) = 0;
};

class EventDispatcher {
public:
    // typedef std::map<EventType, EventTypeMetadata> EventTypeTable;
    typedef std::multimap<EventType, weak_ptr<EventListener> > EventListenerTable;

    EventDispatcher() :
        active_queue_index_(0) {
    }

    bool On(const EventType& type, weak_ptr<EventListener> listener);
    bool Off(const EventType& type);
    bool Off(shared_ptr<EventListener> listener);
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

