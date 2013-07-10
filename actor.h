#ifndef __skidarak__actor__
#define __skidarak__actor__

#include "skidarak/application.h"
#include "skidarak/vector.h"
#include "skidarak/hashed_string.h"
#include "skidarak/actor_id.h"
#include "skidarak/scheduler.h"

#include <memory>

#include <uuid/uuid.h>

namespace skidarak {

typedef HashedString ActorType;

class Actor : public UpdateInterface,
              public enable_shared_from_this<Actor> {
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

    virtual Scheduler& scheduler() const {
        return Application::Instance().director().scheduler();
    }

    virtual EventDispatcher& dispatcher() const {
        return Application::Instance().director().dispatcher();
    }

    void ScheduleUpdate() {
        shared_ptr<UpdateInterface> self = static_pointer_cast<UpdateInterface>(shared_from_this());
        scheduler().Schedule(self);
    }

    void ScheduleUpdate(const ii_time interval) {
        shared_ptr<UpdateInterface> self = static_pointer_cast<UpdateInterface>(shared_from_this());
        scheduler().Schedule(self, interval);
    }

protected:
    ActorId id_;
    HashedString type_;

    vec3 position_;
    vec2 size_;
    float rotation_;
};

}

#endif /* defined(__skidarak__actor__) */
