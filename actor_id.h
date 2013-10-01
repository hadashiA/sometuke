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
    ActorId()
        : uuid_ptr_(nullptr) {
    }

    ~ActorId() {
        if (uuid_ptr_ != nullptr) {
            GeneralMemoryPool::Instance().Free(uuid_ptr_, sizeof(uuid_t));
        }
    }

    void Generate() {
        if (uuid_ptr_ == nullptr) {
            uuid_ptr_ = static_cast<unsigned char *>(GeneralMemoryPool::Instance().Alloc(sizeof(uuid_t)));
            
            uuid_generate(uuid_ptr_);
            uuid_unparse(uuid_ptr_, str_);
        }
    }
    
    const char *str() const {
        return str_;
    }

    bool is_null() const {
        return (!uuid_ptr_ || uuid_is_null(uuid_ptr_));
    }

    bool operator==(const ActorId& rhs) const {
        if (is_null()) {
            return rhs.is_null();
        } else if (rhs.is_null()) {
            return false;
        } else {
            return (uuid_compare(uuid_ptr_, rhs.uuid_ptr_) == 0);
        }
    }

    bool operator!=(const ActorId& rhs) const {
        bool equals = (*this == rhs);
        return !equals;
    }

private:
    unsigned char *uuid_ptr_;
    char str_[37];
};

}

namespace std {
    template <> struct hash<sometuke::ActorId> {
        size_t operator()(const sometuke::ActorId& value) const {
            /* your code here, e.g. "return hash<int>()(x.value);" */
            std::hash<char32_t> hash;
            return hash(reinterpret_cast<char32_t>(value.str()));
        }
    };
}

#endif /* defined(__sometuke__actor_id__) */
