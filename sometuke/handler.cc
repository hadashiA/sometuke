#include "handler.h"

namespace sometuke {

EventListener& Handler::listener() {
    if (!listener_old_) {
        listener_old_ = Pool<EventProxy>(shared_from_this());
    }
    return *listener_old_;
}
    
}
