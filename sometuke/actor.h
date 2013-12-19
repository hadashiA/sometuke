#ifndef __sometuke__actor__
#define __sometuke__actor__

#include "sometuke/director.h"
#include "sometuke/vector.h"

#include <memory>

namespace sometuke {

typedef HashedString ActorType;

class Actor;
class Node;

class Actor {
public:
    static actor_cid NextId() {
        static actor_cid __last_id = 0;
        return ++__last_id;
    }

    Actor(const ActorType& t)
        : type_(t),
          cid_(NextId()) {
    }
    
    virtual ~Actor() {}

    const actor_cid cid() const {
        return cid_;
    }

    const HashedString& type() const {
        return type_;
    }

protected:
    HashedString type_;
};

}

#endif /* defined(__sometuke__actor__) */
