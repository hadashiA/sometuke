#ifndef __kawaii__weak_ptr_hash__
#define __kawaii__weak_ptr_hash__

#include <memory>
#include <functional>

namespace kawaii {
using namespace std;


template<typename T>
struct WeakPtrHash : public unary_function<weak_ptr<T>, size_t> {
   size_t operator()(const weak_ptr<T>& wp) const {
       if (shared_ptr<T> sp = wp.lock()) {
           return std::hash<shared_ptr<T> >()(sp);
       } else {
           return 0;
       }
   }
};

template<typename T>
struct WeakPtrEqual : public unary_function<weak_ptr<T>, bool> {
    bool operator()(const std::weak_ptr<T>& left, const std::weak_ptr<T>& right) const {
        return !left.owner_before(right) && !right.owner_before(left);
    }
};

}

#endif /* defined(__kawaii__weak_ptr_hash__) */
