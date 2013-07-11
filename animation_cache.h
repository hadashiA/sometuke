#ifndef __skidarake__animation_cache__
#define __skidarake__animation_cache__

#include "skidarake/animation.h"

#include <memory>
#include <string>
#include <functional>

namespace skidarake {
using namespace std;

typedef unordered_map<string, shared_ptr<Animation> > AnimationTable;

class AnimationCache {
public:
    static AnimationCache& Instance() {
        static unique_ptr<AnimationCache> __instance(new AnimationCache);
        return *__instance;
    }

    shared_ptr<Animation> operator[](const string& animation_name) {
        return animation_table_[animation_name];
    }

    shared_ptr<Animation> get(const string& name) {
        return animation_table_[name];
    }

    void Add(const string& name, shared_ptr<Animation> animation) {
        animation_table_[name] = animation;
    }

private:
    AnimationCache() {}
    AnimationCache(const AnimationCache&);
    AnimationCache& operator=(const AnimationCache&);

    AnimationTable animation_table_;
};
    
}

#endif /* defined(__skidarake__animation_cache__) */
