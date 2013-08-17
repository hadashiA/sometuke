#ifndef __sometuke__actor__
#define __sometuke__actor__

#include "sometuke/director.h"
#include "sometuke/vector.h"
#include "sometuke/actor_id.h"
#include "sometuke/handler.h"

#include <memory>
#include <uuid/uuid.h>

namespace sometuke {

typedef HashedString ActorType;

struct ActorMoveEvent : public Event {
    static const EventType TYPE;

    ActorMoveEvent(const ActorId& id, const vec3& pos)
        : Event(TYPE),
          actor_id(id),
          location(pos) {
    }

    ActorId actor_id;
    vec3 location;
};

struct ActorRotateEvent : public Event {
    static const EventType TYPE;

    ActorRotateEvent(const ActorId& id, const float degrees)
        : Event(TYPE),
          actor_id(id),
          rotate(degrees) {
    }

    ActorId actor_id;
    float rotate;
};

struct ActorDestroyEvent : public Event {
    static const EventType TYPE;

    ActorDestroyEvent(const ActorId& id)
        : Event(TYPE),
          actor_id(id) {
    }

    ActorId actor_id;
};


class Actor : public Handler {
public:
    Actor(const ActorType& t)
        : type_(t),
          id_(),
          location_(0, 0, 0),
          rotation_(0) {
        id_.Generate();
    }

    virtual ~Actor() {
        Director::Instance().dispatcher().Queue<ActorDestroyEvent>(id_);
    }

    const ActorId& id() const {
        return id_;
    }

    void *id_ptr() {
        return static_cast<void *>(&id_);
    }

    const HashedString& type() const {
        return type_;
    }

    const vec3& location() const {
        return location_;
    }

    const float rotation() const {
        return rotation_;
    }

    void set_location(const float x, const float y) {
        set_location(vec3(x, y, 1));
    }

    void set_location(const vec3& location) {
        if (location_ != location) {
            location_ = location;
            Director::Instance().dispatcher().Queue<ActorMoveEvent>(id_, location);
        }
    }

    void set_rotation(const float rotation) {
        if (rotation_ != rotation) {
            rotation_ = rotation;
            Director::Instance().dispatcher().Queue<ActorRotateEvent>(id_, rotation);
        }
    }

protected:
    ActorId id_;
    HashedString type_;

    vec3 location_;
    float rotation_;
};

}

#endif /* defined(__sometuke__actor__) */
