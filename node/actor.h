#ifndef __kawaii__node_actor__
#define __kawaii__node_actor__

#include "kawaii/utils.h"
#include "kawaii/vector.h"
#include "kawaii/hashed_string.h"
#include "kawaii/node/node.h"

#include <memory>

namespace kawaii {

typedef size_t ActorId;
typedef HashedString ActorType;

class Actor : public Node {
public:
    static hash<string> __hash_func;

    Actor(const HashedString& t)
        : type_(t),
          id_(kawaii::generate_id()) {
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
