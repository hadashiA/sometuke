#ifndef __kawaii__event_dispatcher__
#define __kawaii__event_dispatcher__

#include "hashed_string.h"

#include <memory>
#include <map>

namespace kawaii {
using namespace std;

typedef HashedString EventType;

typedef enum {
    kScriptDefined,             // Event is defined in script.
    kCodeEventOnly,             // Event is defined by main code, and is NOT callable from script.
    kCodeEventScriptCallable,   // Event is defined by code, but is callable from script.
} EventCallable;

struct EventTypeMetadata {
    EvnetCallable callable;
};

class EventListener {
public:
    EventListener() {}
    virtual ~EventListener() {}
    virtual void Callback(const EventType& e) = 0;
};

class EventDispatcher {
public:
    typedef std::map<EventType, EvnetTypeMetadata> EventTypeTable;

    static inline EventDispatcher *Shared() {
        if (__shared == NULL) {
            __shared = new EventDispatcher;
        }
    }

    bool On(const EventType& type, shared_ptr<EventListener> listener);
    bool Off(const EventType& type);
    bool Trigger(const EventType& type);
    bool QueueEvent(const EventType& type);
    bool Tick(const ii_time max_time);
    bool ValidateType(const EventType& type);

private:
    static EventDispatcher *__shared;

    EventDispatcher() {}
};

}

#endif /* defined(__kawaii__event_dispacher__) */
