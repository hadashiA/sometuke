#ifndef __kawaii__ios_touch_dispatcher__
#define __kawaii__ios_touch_dispatcher__

#include "kawaii/ios/events.h"

#include <vector>
#include <memory>

namespace kawaii {
using namespace std;

class TouchListener {
public:
};

class TouchDispatcher {
public:    
    static TouchDispatcher& Instance() {
        static unique_ptr<TouchDispatcher> __instance;
        if (!__instance) {
            __instance.reset(new TouchDispatcher);
        }
        return *__instance;
    }

    void Trigger(vector<TouchEvent> touches);
};

}

#endif /* defined(__kawaii__ios_touch_dispatcher__) */





