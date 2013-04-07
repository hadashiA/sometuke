#ifndef __kawaii__ios_events__
#define __kawaii__ios_events__

#include "event_dispatcher.h"
#include "vector.h"

namespace kawaii {

struct TouchEvent : public Event {
    static const EventType TYPE;

    TouchEvent(const vec2& loc, const vec2& prev, size_t cnt)
    : Event(TYPE),
          location(loc),
          prev_location(prev),
          tap_count(cnt) {
    }

    vec2 location;
    vec2 prev_location;
    size_t tap_count;
};

}

#endif /* defined(__kawaii__ios_events__) */
