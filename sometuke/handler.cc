#include "handler.h"

namespace sometuke {

EventListener& Handler::listener() {
    if (!listener_old_) {
        listener_old_ = Pool<EventProxy>(shared_from_this());
    }
    return *listener_old_;
}

Timer& Handler::timer() {
    if (!timer_) {
        timer_ = Pool<TimerProxy>(shared_from_this());
    }
    return *timer_;
}
    
}
