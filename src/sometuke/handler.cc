#include "handler.h"

namespace sometuke {

EventListener& Handler::listener() {
    if (!listener_) {
        listener_ = Pool<EventProxy>(shared_from_this());
    }
    return *listener_;
}

Timer& Handler::timer() {
    if (!timer_) {
        timer_ = Pool<TimerProxy>(shared_from_this());
    }
    return *timer_;
}
    
}
