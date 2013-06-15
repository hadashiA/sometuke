#ifndef __kawaii__object_id__
#define __kawaii__object_id__

#include "kawaii/memory_pool.h"

#include <string>
#include <memory>
#include <functional>

#include <uuid/uuid.h>
#include <cstdlib>

using namespace std;

namespace kawaii {

class ObjectId {
public:
    ObjectId() {
        uuid_ptr_ = static_cast<unsigned char *>(GeneralMemoryPool::Shared()->Alloc(sizeof(uuid_t)));
        uuid_generate(uuid_ptr_);
    }
    
    ~ObjectId() {
        GeneralMemoryPool::Shared()->Free(uuid_ptr_, sizeof(uuid_t));
    }

    ObjectId(const ObjectId& rhs) {
        uuid_ptr_ = static_cast<unsigned char *>(GeneralMemoryPool::Shared()->Alloc(sizeof(uuid_t)));
        uuid_copy(uuid_ptr_, rhs.handle());
    }

    const string str() const {
        char buf[37];
        uuid_unparse(uuid_ptr_, buf);
        return string(buf);
    }
    
    const unsigned char *handle() const {
        return uuid_ptr_;
    }

    const char *c_str() const {
        return str().c_str();
    }

    bool operator=(const ObjectId& rhs) {
        uuid_copy(uuid_ptr_, rhs.handle());
        return (*this);
    }

    bool operator==(const ObjectId& rhs) {
        return (uuid_compare(uuid_ptr_, rhs.handle()) == 0);
    }

private:
    unsigned char *uuid_ptr_;
};

}

#endif /* defined(__kawaii__object_id__) */
