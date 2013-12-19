#ifndef __sometuke__hashed_string__
#define __sometuke__hashed_string__

#include "sometuke/memory_pool.h"

#include <functional>
#include <iostream>

namespace sometuke {

class HashedString {
public:
    HashedString(const string& str)
        : str_(str),
          id_(0) {

        std::hash<string> h;
        id_ = h(str);
    }

    const int id() const {
        return id_;
    }

    const string str() const {
        return str_;
    }

    const char *c_str() const {
        return str_.c_str();
    }

    bool empty() const {
        return (!str_ || str_[0] == '\0');
    }

    bool operator<(const HashedString& rhs) const {
        return (id_) < rhs.id_));
    }

    bool operator==(const HashedString& rhs) const {
        return (id_) == rhs.id_));
    }

private:
    size_t id_;
    string str_;
};

}

#endif /* defined(__sometuke__hashed_string__) */
