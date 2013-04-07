#include "touch_dispatcher.h"
#include "kawaii/logger.h"

namespace kawaii {

void TouchDispatcher::Trigger(TouchPhase phase, vector<shared_ptr<TouchEvent> > touches) {
    IIINFO("%d", phase);
}

}
