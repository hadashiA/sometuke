#ifndef __sometuke__actor__
#define __sometuke__actor__

#include "sometuke/director.h"
#include "sometuke/vector.h"
#include "sometuke/hashed_string.h"
#include "sometuke/actor_id.h"
#include "sometuke/scheduler.h"

#include <memory>
#include <uuid/uuid.h>

namespace sometuke {

typedef HashedString ActorType;

class Actor : public enable_shared_from_this<Actor> {
public:
    Actor(const ActorType& t)
        : type_(t),
          id_() {
        id_.Generate();
    }

    virtual ~Actor() {}

    const ActorId& id() const {
        return id_;
    }

    void *id_ptr() {
        return static_cast<void *>(&id_);
    }

    const HashedString& type() const {
        return type_;
    }

    const vec3& position() const {
        return position_;
    }

    void set_position(const vec3& position) {
        position_ = position;
    }

    const vec2 size() const {
        return size_;
    }

    void set_size(const vec2& size) {
        size_ = size;
    }

    const float rotation() const {
        return rotation_;
    }

    void set_rotation(const float rotation) {
        rotation_ = rotation;
    }

protected:
    ActorId id_;
    HashedString type_;

    vec3 position_;
    vec2 size_;
    float rotation_;
};

}

#endif /* defined(__sometuke__actor__) */
