#ifndef __sometuke__hashed_string__
#define __sometuke__hashed_string__

#include "sometuke/memory_pool.h"

#include <functional>
#include <iostream>

namespace sometuke {
using namespace std;

class HashedString {
public:
    HashedString(const char *str)
        : str_(nullptr),
          id_(0) {

        str_ = CopyString(str);
        
        const char *ch = str;
        while (*ch)
            id_ = id_ << 1 ^ *ch++;
    }

    ~HashedString() {
        DeleteString(str_);
    }

    const int id() const {
        return id_;
    }

    const char *str() const {
        return str_;
    }

    bool empty() const {
        return (!str_ || str_[0] == '\0');
    }

    bool operator<(const HashedString& rhs) const {
        return (id() < rhs.id());
    }

    bool operator==(const HashedString& rhs) const {
        return (id() == rhs.id());
    }

private:
    int id_;
    char *str_;
};

}

#endif /* defined(__sometuke__hashed_string__) */
