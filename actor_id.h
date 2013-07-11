#ifndef __skidarake__actor_id__
#define __skidarake__actor_id__

#include "skidarake/memory_pool.h"

#include <string>
#include <memory>
#include <functional>

#include <uuid/uuid.h>
#include <cstdlib>
#include <iostream>

using namespace std;

namespace skidarake {

class ActorId {
public:
    struct UuidDeleter { 
        void operator()(unsigned char* p) const {
            GeneralMemoryPool::Shared()->Free(p, sizeof(uuid_t));
            p = nullptr;
        }
    };

    ActorId()
        : uuid_ptr_(nullptr) {
    }

    void Generate() {
        if (!uuid_ptr_) {
            unsigned char *ptr =
                static_cast<unsigned char *>(GeneralMemoryPool::Shared()->Alloc(sizeof(uuid_t)));
            
            uuid_generate(ptr);
            uuid_ptr_.reset(ptr, UuidDeleter());
        }
    }

    const string str() const {
        char buf[37];
        uuid_unparse(uuid_ptr_.get(), buf);
        return string(buf);
    }
    
    const char *c_str() const {
        return str().c_str();
    }

    bool is_null() const {
        return uuid_is_null(uuid_ptr_.get());
    }

    bool operator==(const ActorId& rhs) const {
        return (uuid_compare(uuid_ptr_.get(), rhs.uuid_ptr_.get()) == 0);
    }

private:

    shared_ptr<unsigned char> uuid_ptr_;
};

struct ActorIdHash : public unary_function<ActorId, size_t> {
    size_t operator()(const ActorId& value) const {
        std::hash<string> hash;
        return hash(value.str());
    }
};

}

#endif /* defined(__skidarake__actor_id__) */
