#ifndef __kawaii__node_actor__
#define __kawaii__node_actor__

#include "kawaii/vector.h"
#include "kawaii/hashed_string.h"
#include "kawaii/node/node.h"

#include <memory>

#include <uuid/uuid.h>

namespace kawaii {

typedef size_t ActorId;
typedef HashedString ActorType;

class Actor : public Node {
public:
    static inline ActorId GenerateId() {
        unsigned long number;
        
        uuid_t uuid;
        uuid_generate(uuid);
        memcpy(&number, uuid, sizeof(number));
        
        return number;
    }

    Actor(const HashedString& t)
        : type_(t),
          id_(Actor::GenerateId()) {
    }

    virtual ~Actor() {}

    const ActorId id() const {
        return id_;
    }

    const HashedString type() const {
        return type_;
    }

protected:
    ActorId id_;
    HashedString type_;
};

}

#endif /* defined(__kawaii__node_actor__) */
