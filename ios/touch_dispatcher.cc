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
    standard_listeners_.clear();
    targeted_listeners_.clear();
}

void TouchDispatcher::RemoveListener(weak_ptr<StandardTouchListener> listener) {
    shared_ptr<StandardTouchListener> listener_ptr = listener.lock();
    for (StandardTouchListenerTable::iterator i = standard_listeners_.begin();
         i != standard_listeners_.end();) {
        shared_ptr<StandardTouchListener> table_listener_ptr = i->second.lock();
        if (!table_listener_ptr || listener_ptr == table_listener_ptr) {
            standard_listeners_.erase(i++);
        } else {
            ++i;
        }
    }
}

void TouchDispatcher::RemoveListener(weak_ptr<TargetedTouchListener> listener) {
    shared_ptr<TargetedTouchListener> listener_ptr = listener.lock();
    for (TargetedTouchListenerTable::iterator i = targeted_listeners_.begin();
         i != targeted_listeners_.end();) {
        shared_ptr<TargetedTouchListener> table_listener_ptr = i->second.lock();
        if (!table_listener_ptr || listener_ptr == table_listener_ptr) {
            targeted_listeners_.erase(i++);
        } else {
            ++i;
        }
    }
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
