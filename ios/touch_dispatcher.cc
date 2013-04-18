#include "touch_dispatcher.h"

#include "kawaii/logger.h"
#include "kawaii/node/node.h"

namespace kawaii {

void TouchDispatcher::AddStandardListener(const StandardTouchListener& listener, int priority) {
    pair<int, weak_ptr<StandardTouchListener> > val(priority, listener);
    standard_listeners_.insert(val);
}

void TouchDispatcher::AddTargetedListener(const TargetedTouchListener& listener, int priority) {
    pair<int, weak_ptr<TargetedTouchListener> > val(priority, listener);
    targeted_listeners_.insert(val);
}

void TouchDispatcher::RemoveListener(const StandardTouchListener& listener) {
    for (StandardTouchListenerTable::iterator i = standard_listeners_.begin();
         i != standard_listeners_.end();) {
        if (listener.handler() == i->second.handler()) {
            standard_listeners_.erase(i++);
        } else {
            ++i;
        }
    }
}

void TouchDispatcher::RemoveListener(const TargetedTouchListener& listener) {
    for (TargetedTouchListenerTable::iterator i = targeted_listeners_.begin();
         i != targeted_listeners_.end();) {
        if (listener.handler() == i->second.handler()) {
            targeted_listeners_.erase(i++);
        } else {
            ++i;
        }
    }
}

void TouchDispatcher::Trigger(TouchPhase phase, TouchEventSet touches) {
    for (TouchEventSet::iterator touch_iter = touches.begin(); touch_iter != touches.end(); ++touch_iter) {
        shared_ptr<TouchEvent> touch = *touch_iter;

        // process the target handlers 1st
        for (TargetedTouchListenerTable::iterator i = targeted_listeners_.begin();
             i != targeted_listeners_.end();) {
            TargetedTouchListener& listener = i->second;
            if (listener.handler()) {
                bool claimed = false;
                if (phase == kTouchBegan) {
                    listener.TouchStart(touch);
                } else {
                }

                ++i;
            } else {
                targeted_listeners_.erase(i++);
            }
        }

        // process standard handlers 2nd
        
    }
}

}
