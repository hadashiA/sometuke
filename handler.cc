#include "handler.h"

namespace sometuke {

EventListener& Handler::listener() {
    if (!listener_) {
        listener_ = New<EventProxy>(shared_from_this());
    }
    return *listener_;
}

Timer& Handler::timer() {
    if (!timer_) {
        timer_ = New<TimerProxy>(shared_from_this());
    }
    return *timer_;
}
    
}
