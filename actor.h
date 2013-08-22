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

struct ActorAddEvent : public Event {
    static const EventType TYPE;

    ActorAddEvent(const ActorId& id, const ActorType& t)
        : Event(TYPE),
          actor_id(id),
          type(t) {
    }

    ActorId actor_id;
    ActorType type;
};

struct ActorRemoveEvent : public Event {
    static const EventType TYPE;

    ActorRemoveEvent(const ActorId& id)
        : Event(TYPE),
          actor_id(id) {
    }

    ActorId actor_id;
};

class Actor : public Handler {
public:
    Actor(const ActorType& t)
        : type_(t),
          id_()
        id_.Generate();
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

protected:
    ActorId id_;
    HashedString type_;
};

}

#endif /* defined(__sometuke__actor__) */
