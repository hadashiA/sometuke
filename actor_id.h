#ifndef __sometuke__actor_id__
#define __sometuke__actor_id__

#include "sometuke/memory_pool.h"

#include <string>
#include <memory>
#include <functional>

#include <uuid/uuid.h>
#include <cstdlib>
#include <iostream>

using namespace std;

namespace sometuke {

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
        return (!uuid_ptr_ || uuid_is_null(uuid_ptr_.get()));
    }

    bool operator==(const ActorId& rhs) const {
        return (uuid_compare(uuid_ptr_.get(), rhs.uuid_ptr_.get()) == 0);
    }

private:
    shared_ptr<unsigned char> uuid_ptr_;
};

}

namespace std {
    template <> struct hash<sometuke::ActorId>
    {
        size_t operator()(const sometuke::ActorId& value) const {
            /* your code here, e.g. "return hash<int>()(x.value);" */
            std::hash<string> hash;
            return hash(value.str());
        }
    };
}

#endif /* defined(__sometuke__actor_id__) */
