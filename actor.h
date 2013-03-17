#ifndef __kawaii__actor__
#define __kawaii__actor__

#include "vector.h"
#include "hashed_string.h"

#include <memory>

namespace kawaii {

typedef unsigned int actor_id;

class Actor;
class Node;

struct ActorStatus : public enable_shared_from_this<ActorStatus> {
    static int NextId() {
        static int __last_id = 0;
        return ++__last_id;
    }

    ActorStatus(const HashedString& t, const vec3 p = vec3(0, 0, 0))
        : type(t),
          position(p) {
        id = ActorStatus::NextId();
    }

    virtual ~ActorStatus() {}

    virtual bool Appear() = 0;  // create Actor,Node,

    actor_id id;
    HashedString type;
    vec3 position;
};

class Actor {
public:
    Actor(shared_ptr<ActorStatus> status) :
        status_(status) {
    }

    const actor_id id() const {
        return status_->id;
    }

    const shared_ptr<ActorStatus>& status() const {
        return status_;
    }

private:
    shared_ptr<ActorStatus> status_;
};

}

#endif /* defined(__kawaii__actor__) */
