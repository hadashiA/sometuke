#include "handler.h"

namespace sometuke {

EventListener& Handler::listener() {
    if (!listener_) {
        listener_ = New<EventAdapter>(shared_from_this());
    }
    return *listener_;
}

Timer& Handler::timer() {
    if (!timer_) {
        timer_ = New<TimerAdapter>(shared_from_this());
    }
    return *timer_;
}
    
}
