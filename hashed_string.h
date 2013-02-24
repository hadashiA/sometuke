#ifndef __kawaii__hashed_string__
#define __kawaii__hashed_string__

#include <string>

namespace kawaii {
using namespace std;

#pragma warning(push)
#pragma warning(disable : 4311)

class HashedString {
public:
    static void *HashName(const char *ident_c_str);

    HashedString(const char *const ident_c_str)
        : ident_(HashName(ident_c_str)),
          ident_string_(ident_c_str) {}

    HashedString(const string& ident_string)
        : ident_(HashName(ident_string.c_str())),
          ident_string_(ident_string) {}

    const unsigned long id() const {
        return reinterpret_cast<unsigned long>(ident_);
    }

    const string& string() const {
        return ident_string_;
    }

    bool empty() const {
        return string().empty();
    }

    bool operator<(const HashedString& rhs) const {
        return (id() < rhs.id());
    }

    bool operator==(const HashedString& rhs) const {
        return (id() == rhs.id());
    }

private:
    void *ident_;
    std::string ident_string_;
};

#pragma warning(pop)

}

#endif /* defined(__kawaii__hashed_string__) */
