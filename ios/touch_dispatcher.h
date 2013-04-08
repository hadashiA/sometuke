#ifndef __kawaii__ios_touch_dispatcher__
#define __kawaii__ios_touch_dispatcher__

#include "kawaii/ios/events.h"

#include <vector>
#include <list>
#include <memory>

namespace kawaii {
using namespace std;

typedef vector<shared_ptr<TouchEvent> > TouchEventSet;

class TargetTouchListener {
public:
    virtual ~TargetTouchListener() {}
    virtual bool TouchBegan(shared_ptr<TouchEvent> touch) = 0;
    virtual bool TouchMoved(shared_ptr<TouchEvent> touch) = 0;
    virtual bool TouchEnded(shared_ptr<TouchEvent> touch) = 0;
    virtual bool TouchCancelled(shared_ptr<TouchEvent> touch) = 0;
};

class StandardTouchListener {
public:
    virtual ~StandardTouchListener() {}
    virtual bool TouchesBegan(TouchEventSet touches) = 0;
    virtual bool TouchesMoved(TouchEventSet touches) = 0;
    virtual bool TouchesEnded(TouchEventSet touches) = 0;
    virtual bool TouchesCancelled(TouchEventSet touches) = 0;
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

    void AddStandardListener(weak_ptr<StandardTouchListener> listener);
    void AddTargetListener(weak_ptr<TargetTouchListener> listener);

    void Trigger(TouchPhase phase, TouchEventSet touches);

private:
    bool enabled_;
    list<shared_ptr<TargetTouchListener> > targeted_listeners_;
    list<shared_ptr<StandardTouchListener> > standard_listeners_;
};

}

#endif /* defined(__kawaii__ios_touch_dispatcher__) */
