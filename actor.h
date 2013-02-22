#ifndef __kawaii__actor__
#define __kawaii__actor__

#include "vector.h"
#include "hashed_string.h"

#include <memory>

namespace kawaii {

typedef unsigned int actor_id;

class Actor;

struct ActorStatus {
    static int NextId() {
        static int __last_id = 0;
        return ++__last_id;
    }

    ActorStatus(const HashedString& t, const vec3 p = vec3(0, 0, 0))
        : type(t),
          position(p) {
        id = ActorStatus::NextId();
    }

    actor_id id;
    HashedString type;
    vec3 position;
};

class Actor {
public:
    const actor_id id() const {
        return status_->id;
    }

    const unique_ptr<ActorStatus>& status() const {
        return status_;
    }

private:
    unique_ptr<ActorStatus> status_;
};

}

#endif /* defined(__kawaii__actor__) */
