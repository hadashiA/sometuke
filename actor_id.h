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
            S2Free(p);
            p = nullptr;
        }
    };

    ActorId()
        : uuid_ptr_(nullptr) {
    }

    void Generate() {
        if (!uuid_ptr_) {
            unsigned char *ptr =
                static_cast<unsigned char *>(S2Alloc(sizeof(uuid_t), __FILE__, __LINE__));
            
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
