#ifndef __kawaii__events__
#define __kawaii__events__

#include "kawaii/memory_pool.h"
#include "kawaii/hashed_string.h"

namespace kawaii {

typedef HashedString EventType;

struct Event : public GeneralPoolable {
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

#endif /* defined(__kawaii__events__) */
