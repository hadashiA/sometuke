#include "touch_dispatcher.h"
#include "kawaii/logger.h"

namespace kawaii {

void TouchDispatcher::AddStandardListener(weak_ptr<StandardTouchListener> listener,
                                          int priority) {
    pair<int, weak_ptr<StandardTouchListener> > val(priority, listener);
    standard_listeners_.insert(val);
}

void TouchDispatcher::AddTargetedListener(weak_ptr<TargetedTouchListener> listener,
                                          int priority) {
    pair<int, weak_ptr<TargetedTouchListener> > val(priority, listener);
    targeted_listeners_.insert(val);
}

void TouchDispatcher::RemoveAllListeners() {
}

void RemoveListener(weak_ptr<StandardTouchListener> listener) {
}

void RemoveListener(weak_ptr<TargetedTouchListener> listener) {
}

void TouchDispatcher::Trigger(TouchPhase phase, TouchEventSet touches) {
    for (TouchEventSet::iterator i = touches.begin(); i != touches.end(); ++i) {
        shared_ptr<TouchEvent> touch = *i;
        IIINFO("touch:%d location:%s",
               touch->phase,
               IIINSPECT(touch->location)
               );
    }
}

}





