#ifndef __kawaii__actor_id__
#define __kawaii__actor_id__

#include "kawaii/memory_pool.h"

#include <string>
#include <memory>
#include <functional>

#include <uuid/uuid.h>
#include <cstdlib>

using namespace std;

namespace kawaii {

class ActorId {
public:
    ActorId()
        : uuid_ptr(nullptr) {
        Alloc();
        uuid_generate(uuid_ptr);
    }
    
    ~ActorId() {
        Free();
    }

    ActorId(const ActorId& rhs)
        : uuid_ptr(nullptr) {
        Alloc();
        uuid_copy(uuid_ptr, rhs.uuid_ptr);
    }

    ActorId(ActorId&& rhs) {
        uuid_ptr = rhs.uuid_ptr;
        rhs.uuid_ptr = nullptr;
    }

    const string str() const {
        char buf[37];
        uuid_unparse(uuid_ptr, buf);
        return string(buf);
    }
    
    const char *c_str() const {
        return str().c_str();
    }

    bool is_null() const {
        return uuid_is_null(uuid_ptr);
    }

    ActorId& operator=(const ActorId& rhs) {
        uuid_copy(uuid_ptr, rhs.uuid_ptr);
        return (*this);
    }

    ActorId& operator=(ActorId&& rhs) {
        uuid_ptr = rhs.uuid_ptr;
        rhs.uuid_ptr = nullptr;
        return (*this);
    }

    bool operator==(const ActorId& rhs) const {
        return (uuid_compare(uuid_ptr, rhs.uuid_ptr) == 0);
    }


    unsigned char *uuid_ptr;

private:
    void Alloc() {
        if (uuid_ptr == nullptr) {
            uuid_ptr = static_cast<unsigned char *>(GeneralMemoryPool::Shared()->Alloc(sizeof(uuid_t)));
        }
    }

    void Free() {
        if (uuid_ptr != nullptr) {
            GeneralMemoryPool::Shared()->Free(uuid_ptr, sizeof(uuid_t));
            uuid_ptr = nullptr;
        }
    }
};

struct ActorIdHash : public unary_function<ActorId, size_t> {
    size_t operator()(const ActorId& value) const {
        std::hash<string> hash;
        return hash(value.str());
    }
};

}

#endif /* defined(__kawaii__actor_id__) */
