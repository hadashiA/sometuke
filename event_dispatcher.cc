#include "event_dispatcher.h"

#include <functional>
// #include <cstring>

namespace kawaii {

bool EventDispatcher::On(const EventType& type, EventListener *listener) {
    std::pair<EventType, EventListener *> pair(type, listener);
    listeners_.insert(pair);
    return true;
}

bool EventDispatcher::Off(const EventType& type) {
    return true;
}

bool EventDispatcher::Off(const EventListener *listener) {
    return true;
}

void EventDispatcher::Trigger(const Event& event) {
    std::pair<EventListenerTable::iterator, EventListenerTable::iterator> range =
        listeners_.equal_range(event.type);
    for (EventListenerTable::iterator i = range.first; i != range.second; ++i) {
        EventListener *listener = i->second;
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
