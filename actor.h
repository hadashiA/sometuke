#ifndef __kawaii__actor__
#define __kawaii__actor__

#include "vector.h"
#include "hashed_string.h"

namespace kawaii {

typedef unsigned int actor_id;

struct ActorStatus {
    actor_id id;
    HashedString type;
    vec3 position;
};

class Actor {
public:
    static int NextId() {
        static int __last_id = 0;
        return ++__last_id;
    }

    ~Actor() {
        delete status_;
    }

    const actor_id id() const {
        return status_->id;
    }

    const ActorStatus& status() const {
        return *status_;
    }

private:
     ActorStatus *status_;
};

}

#endif /* defined(__kawaii__actor__) */
