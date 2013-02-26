#include "event_dispatcher.h"

#include <functional>
#include <cassert>
// #include <cstring>

namespace kawaii {

bool EventDispatcher::On(const EventType& type, EventListenerInterface *listener) {
    if (!IsValidType(type)) {
        return false;
    }

    listeners_.insert(std::make_pair(type, listener));
    return true;
}

bool EventDispatcher::Off(const EventType& type) {
    std::pair<EventListenerTable::iterator, EventListenerTable::iterator> range =
        listeners_.equal_range(type);

    listeners_.erase(range.first, range.second);

    return true;
}

bool EventDispatcher::Off(EventListenerInterface *listener) {
    for (EventListenerTable::iterator i = listeners_.begin(); i != listeners_.end();) {
        if (listener == i->second) {
            listeners_.erase(i++);
        } else {
            ++i;
        }
    }
    return true;
}

bool EventDispatcher::Trigger(shared_ptr<Event> event) {
    const EventType& type = event->type;

    if (!IsValidType(type) || !IsListerningType(type)) {
        return false;
    }

    bool emitted = false;

    std::pair<EventListenerTable::iterator, EventListenerTable::iterator> range =
        listeners_.equal_range(type);
    for (EventListenerTable::iterator i = range.first; i != range.second;) {
        emitted = true;
        
        (i->second)->HandleEvent(event);
        ++i;
    }

    return emitted;
}

bool EventDispatcher::Queue(shared_ptr<Event> event) {
    assert(active_queue_index_ >= 0);
    assert(active_queue_index_ < NUM_QUEUES);

    if (!IsValidType(event->type) || !IsListerningType(event->type)) {
        return false;
    }

    queues_[active_queue_index_].push_back(event);

    return true;
}

bool EventDispatcher::Tick(const ii_time max_time) {
    std::list<shared_ptr<Event> > queue = queues_[active_queue_index_];

    // swap active queues, make sure new queue is empty after the swap..
    active_queue_index_ = (active_queue_index_ + 1) % NUM_QUEUES;
    queues_[active_queue_index_].clear();

    while (!queue.empty()) {
        shared_ptr<Event> event = queue.front();
        queue.pop_front();

        std::pair<EventListenerTable::iterator, EventListenerTable::iterator> range =
            listeners_.equal_range(event->type);
        for (EventListenerTable::iterator i = range.first; i != range.second;) {
            EventListenerInterface *listener = i->second;
            if (listener->HandleEvent(event)) {
                break;
            }
            ++i;
        }
    }

    // if any events left to process, push them onto the active queue.
    bool queue_flushed = queue.empty();
    if (!queue_flushed) {
        while (!queue.empty()) {
            queues_[active_queue_index_].push_front(queue.back());
            queue.pop_back();
        }
    }
    
    return queue_flushed;
}

bool EventDispatcher::IsValidType(const EventType& type) const {
    return !type.string().empty();
}

bool EventDispatcher::IsListerningType(const EventType& type) const {
    return (listeners_.find(type) != listeners_.end());
}

}
