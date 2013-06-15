#ifndef __kawaii__uuid__
#define __kawaii__uuid__

#include "kawaii/memory_pool.h"

#include <string>
#include <memory>
#include <functional>

#include <uuid/uuid.h>
#include <cstdlib>

using namespace std;

namespace kawaii {

class Uuid {
public:
    Uuid() {
        uuid_ptr_ = static_cast<unsigned char *>(GeneralMemoryPool::Shared()->Alloc(sizeof(uuid_t)));
        uuid_generate(uuid_ptr_);
    }
    
    ~Uuid() {
        GeneralMemoryPool::Shared()->Free(uuid_ptr_, sizeof(uuid_t));
    }

    Uuid(const Uuid& rhs) {
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

    Uuid& operator=(const Uuid& rhs) {
        uuid_copy(uuid_ptr_, rhs.handle());
        return (*this);
    }

    bool operator==(const Uuid& rhs) const {
        return (uuid_compare(uuid_ptr_, rhs.handle()) == 0);
    }

private:
    unsigned char *uuid_ptr_;
};

struct UuidHash : public unary_function<Uuid, size_t> {
    size_t operator()(const Uuid& value) const {
        std::hash<string> hash;
        return hash(value.str());
    }
};

struct UuidEqual : public unary_function<Uuid, bool> {
    bool operator()(const Uuid& lhs, const Uuid& rhs) const {
        return (lhs == rhs);
    }
};

}

#endif /* defined(__kawaii__uuid__) */
