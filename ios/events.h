#ifndef __kawaii__ios_events__
#define __kawaii__ios_events__

#include "event_dispatcher.h"

namespace kawaii {

struct TouchesBeganEvent : public Event {
    static const EventType TYPE;

    TouchesBeganEvent() : Event(TYPE) {}
};

struct TouchesMovedEvent : public Event {
    static const EventType TYPE;

    TouchesMovedEvent() : Event(TYPE) {}
};

struct TouchesEndedEvent : public Event {
    static const EventType TYPE;

    TouchesEndedEvent() : Event(TYPE) {}
};

struct TouchesCancelledEvent : public Event {
    static const EventType TYPE;

    TouchesCancelledEvent() : Event(TYPE) {}
};

}

#endif /* defined(__kawaii__ios_events__) */
