#ifndef __sometuke__actor_id__
#define __sometuke__actor_id__

#include "sometuke/memory_pool.h"
#include "sometuke/logger.h"

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
            free(p);
            p = nullptr;
        }
    };

    ActorId()
        : uuid_ptr_(nullptr) {
    }

    void Generate() {
        if (!uuid_ptr_) {
            unsigned char *ptr = static_cast<unsigned char *>(malloc(sizeof(uuid_t)));
            
            uuid_ptr_.reset(ptr, UuidDeleter());
            uuid_unparse(ptr, str_);
        }
    }

    const char *str() const {
        return str_;
    }
    
    bool is_null() const {
        return (!uuid_ptr_ || uuid_is_null(uuid_ptr_.get()));
    }

    bool operator==(const ActorId& rhs) const {
        if (is_null()) {
            return rhs.is_null();
        } else if (rhs.is_null()) {
            return false;
        } else {
            return (uuid_compare(uuid_ptr_.get(), rhs.uuid_ptr_.get()) == 0);
        }
    }

    bool operator!=(const ActorId& rhs) const {
        bool equals = (*this == rhs);
        return !equals;
    }

private:
    shared_ptr<unsigned char> uuid_ptr_;
    char str_[37];
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
