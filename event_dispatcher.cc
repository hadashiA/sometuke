#include "event_dispatcher.h"

#include <cstring>

namespace kawaii {

bool EventDispatcher::ValidateType(const EventType& type) {
    if (type.string().empty()) {
        return false;
    }

    if (type.id() == 0 && strcmp(type.string().c_str(), "*") != 0) {
        return false;
    }
}

}
