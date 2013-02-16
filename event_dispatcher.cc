#include "event_dispatcher.h"

#include <functional>
// #include <cstring>

namespace kawaii {

bool EventDispatcher::On(const EventType& type, shared_ptr<EventListener> listener) {
    std::pair<EventType, shared_ptr<EventListener> > pair(type, listener);
    listeners_.insert(pair);
    return true;
}

bool EventDispatcher::Off(const EventType& type) {
    return true;
}

bool EventDispatcher::Off(shared_ptr<EventListener> listener) {
    return true;
}

void EventDispatcher::Trigger(const Event& event) {
    std::pair<EventListenerTable::iterator, EventListenerTable::iterator> range =
        listeners_.equal_range(event.type);
    for (EventListenerTable::iterator i = range.first; i != range.second; ++i) {
        shared_ptr<EventListener> listener = i->second;
        listener->Callback(event);
    }
}

bool EventDispatcher::Queue(const Event& event) {
    return true;
}

bool EventDispatcher::Tick(const ii_time max_time) {
    return true;
}

}
