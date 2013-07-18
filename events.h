#ifndef __skidarake__events__
#define __skidarake__events__

#include "skidarake/memory_pool.h"
#include "skidarake/hashed_string.h"
#include "skidarake/actor_id.h"
#include "skidarake/vector.h"

#include <memory>

namespace skidarake {

typedef HashedString EventType;

struct Event {
    Event(const EventType& t)
        : type(t),
          timestamp(std::time(NULL)) {
    }

    Event(const string& str)
        : type(EventType(str)),
          timestamp(std::time(NULL)) {
    }

    virtual ~Event() {}

    EventType type;
    time_t timestamp;
};

}

#endif /* defined(__skidarake__events__) */
