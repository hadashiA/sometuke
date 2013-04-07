#include "touch_dispatcher.h"
#include "kawaii/logger.h"

namespace kawaii {

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





