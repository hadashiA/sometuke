#include "event_dispatcher.h"

#include <functional>
// #include <cstring>

namespace kawaii {

bool EventDispatcher::On(const EventType& type, weak_ptr<EventListener> listener) {
    weak_ptr<EventListener> listener_ref = listener;
    std::pair<EventType, weak_ptr<EventListener> > pair(type, listener_ref);
    listeners_.insert(pair);
    return true;
}

bool EventDispatcher::Off(const EventType& type) {
    std::pair<EventListenerTable::iterator, EventListenerTable::iterator> range =
        listeners_.equal_range(type);

    listeners_.erase(range.first, range.second);

    return true;
}

bool EventDispatcher::Off(shared_ptr<EventListener> listener) {
    for (EventListenerTable::iterator i = listeners_.begin(); i != listeners_.end();) {
        weak_ptr<EventListener> ref = i->second;
        if (listener == ref.lock()) {
            listeners_.erase(i++);
        } else {
            ++i;
        }
    }

    return true;
}

void EventDispatcher::Trigger(const Event& event) {
    std::pair<EventListenerTable::iterator, EventListenerTable::iterator> range =
        listeners_.equal_range(event.type);
    for (EventListenerTable::iterator i = range.first; i != range.second;) {
        weak_ptr<EventListener> listener_ref = i->second;
        if (shared_ptr<EventListener> listener = listener_ref.lock()) {
            listener->EventHandle(event);
            ++i;
        } else {
            listeners_.erase(i++);
        }
    }
}

bool EventDispatcher::Queue(const Event& event) {
    return true;
}

bool EventDispatcher::Tick(const ii_time max_time) {
    return true;
}

}
