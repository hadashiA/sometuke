#ifndef __kawaii__ios_touch_dispatcher__
#define __kawaii__ios_touch_dispatcher__

#include "kawaii/ios/events.h"

#include <vector>
#include <map>
#include <memory>

namespace kawaii {
using namespace std;

typedef vector<shared_ptr<TouchEvent> > TouchEventSet;
    
class TargetedTouchListener {
public:
    virtual ~TargetedTouchListener() {}
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
    
typedef multimap<int, weak_ptr<StandardTouchListener> > StandardTouchListenerTable;
typedef multimap<int, weak_ptr<TargetedTouchListener> > TargetedTouchListenerTable;    

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

    void AddStandardListener(weak_ptr<StandardTouchListener> listener, int priority);
    void AddTargetedListener(weak_ptr<TargetedTouchListener> listener, int priority);

    void RemoveAllListeners();
    void RemoveListener(weak_ptr<StandardTouchListener> listener);
    void RemoveListener(weak_ptr<TargetedTouchListener> listener);

    void Trigger(TouchPhase phase, TouchEventSet touches);

private:
    bool enabled_;
    TargetedTouchListenerTable targeted_listeners_;
    StandardTouchListenerTable standard_listeners_;
};

}

#endif /* defined(__kawaii__ios_touch_dispatcher__) */
