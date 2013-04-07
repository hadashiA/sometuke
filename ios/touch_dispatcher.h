#ifndef __kawaii__ios_touch_dispatcher__
#define __kawaii__ios_touch_dispatcher__

#include "kawaii/ios/events.h"

#include <vector>
#include <list>
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

    TouchDispatcher() : enabled_(true) {
    }

    bool enabled() { return enabled_; }
    void Enable()  { enabled_ = true; }
    void Disable() { enabled_ = false; }
    void Trigger(TouchPhase phase, vector<shared_ptr<TouchEvent> > touches);

private:
    bool enabled_;
    list<shared_ptr<TouchListener> > targeted_listeners_;
    list<shared_ptr<TouchListener> > standard_listeners_;
};

}

#endif /* defined(__kawaii__ios_touch_dispatcher__) */
