#ifndef __kawaii__events__
#define __kawaii__events__

#include "kawaii/memory_pool.h"
#include "kawaii/hashed_string.h"
#include "kawaii/actor_id.h"
#include "kawaii/vector.h"

#include <memory>

namespace kawaii {

typedef HashedString EventType;

struct Event : public GeneralPoolable {
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

template<class T>
shared_ptr<T> CreateEvent() {
    shared_ptr<T> ptr(new T);
    return ptr;
}

template<class T, class Arg1, class... Args>
shared_ptr<T> CreateEvent(Arg1& arg1, Args& ... args) {
    shared_ptr<T> ptr(new T(arg1, args...));
    return ptr;
}

}

#endif /* defined(__kawaii__events__) */
