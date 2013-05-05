#ifndef __kawaii__hashed_string__
#define __kawaii__hashed_string__

#include <string>
#include <functional>
#include <iostream>

namespace kawaii {
using namespace std;

class HashedString {
public:
    static size_t Hash(const string& s) {
        static hash<string> __hash_func;
        return __hash_func(s);
    }

    HashedString(const string& ident_string)
        : ident_string_(ident_string) {
        ident_ = Hash(ident_string_);
    }

    const unsigned long id() const {
        return reinterpret_cast<unsigned long>(ident_);
    }

    const string& str() const {
        return ident_string_;
    }

    const char *c_str() const {
        return ident_string_.c_str();
    }

    bool empty() const {
        return str().empty();
    }

    bool operator<(const HashedString& rhs) const {
        return (id() < rhs.id());
    }

    bool operator==(const HashedString& rhs) const {
        return (id() == rhs.id());
    }

private:
    // unsigned long ident_;
    size_t ident_;
    string ident_string_;
};

}

#endif /* defined(__kawaii__hashed_string__) */
