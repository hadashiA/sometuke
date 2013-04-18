#ifndef __kawaii__ios_events__
#define __kawaii__ios_events__

#include "kawaii/event_dispatcher.h"
#include "kawaii/vector.h"

namespace kawaii {

typedef unsigned long TouchId;

typedef enum {
    kTouchBegan,
    kTouchMoved,
    kTouchEnded,
    kTouchCancelled,
} TouchPhase;

struct TouchEvent : public Event {
    static const EventType TYPE;

    TouchEvent(TouchId touch_id, TouchPhase _phase, const vec2& loc, const vec2& prev, size_t cnt)
        : Event(TYPE),
          id(touch_id),
          phase(_phase),
          location(loc),
          prev_location(prev),
          tap_count(cnt) {
    }

    TouchId id;
    TouchPhase phase;
    vec2 location;
    vec2 prev_location;
    size_t tap_count;
};

}

#endif /* defined(__kawaii__ios_events__) */
