#ifndef __skidarak__events__
#define __skidarak__events__

#include "skidarak/memory_pool.h"
#include "skidarak/hashed_string.h"
#include "skidarak/actor_id.h"
#include "skidarak/vector.h"

#include <memory>

namespace skidarak {

typedef HashedString EventType;

struct Event {
    Event(const EventType& t)
        : type(t),
          timestamp(std::time(NULL)) {
    }

    Event(const string& str)
        : type(EventType(str)),
          timestamp(std::time(NULL)) {
    }

    virtual ~Event() {}

    EventType type;
    time_t timestamp;
};

struct MoveActorEvent : public Event {
    static const EventType TYPE;

    MoveActorEvent(const ActorId& id, const vec2& pos)
        : Event(TYPE),
          actor_id(id),
          position(pos) {
    }

    ActorId actor_id;
    vec2 position;
};

struct RotateActorEvent : public Event {
    static const EventType TYPE;

    RotateActorEvent(const ActorId& id, const float degrees)
        : Event(TYPE),
          actor_id(id),
          rotate(degrees) {
    }

    ActorId actor_id;
    float rotate;
};

struct AccelerationEvent : public Event {
    static const EventType TYPE;

    AccelerationEvent(const vec3& v)
        : Event(TYPE),
          gravity(v) {
    }

    vec3 gravity;
};

}

#endif /* defined(__skidarak__events__) */
