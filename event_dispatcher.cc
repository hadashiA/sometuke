#include "sometuke/event_dispatcher.h"

#include "sometuke/director.h"
#include "sometuke/logger.h"

#include <functional>
#include <cassert>

namespace sometuke {

bool EventListener::ListenTo(const EventType& type) {
    return Director::Instance().dispatcher().On(type, shared_from_this());
}

bool EventListener::StopListering() {
    return Director::Instance().dispatcher().Off(shared_from_this());
}

bool EventDispatcher::On(const EventType& type, shared_ptr<EventListener> listener) {
    if (!IsValidType(type)) {
        return false;
    }

    pair<EventType, shared_ptr<EventListener> > pair(type, listener);
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
        if (listener == i->second) {
            listeners_.erase(i++);
        } else {
            ++i;
        }
    }
    return true;
}

bool EventDispatcher::Off(const EventType& type, shared_ptr<EventListener> listener) {
    pair<EventListenerTable::iterator, EventListenerTable::iterator> range =
        listeners_.equal_range(type);
    for (EventListenerTable::iterator i = range.first; i != range.second;) {
        if (listener == i->second) {
            listeners_.erase(i++);
        } else {
            ++i;
        }
    }
    return true;
}

bool EventDispatcher::Trigger(const shared_ptr<Event>& event) {
    const EventType& type = event->type;

    if (!IsValidType(type) || !IsListerningType(type)) {
        return false;
    }

    bool emitted = false;

    pair<EventListenerTable::iterator, EventListenerTable::iterator> range =
        listeners_.equal_range(type);
    for (EventListenerTable::iterator i = range.first; i != range.second;) {
        shared_ptr<EventListener> listener = i->second;
        if (!listener->paused()) {
            emitted = true;
            if (!listener->HandleEvent(event)) {
                listeners_.erase(i++);
            }
        }
        ++i;
    }

    return emitted;
}

bool EventDispatcher::Queue(const shared_ptr<Event>& event) {
    assert(active_queue_index_ >= 0);
    assert(active_queue_index_ < NUM_QUEUES);

    if (!IsValidType(event->type) || !IsListerningType(event->type)) {
        return false;
    }

    queues_[active_queue_index_].push_back(event);

    return true;
}

bool EventDispatcher::Tick(const s2_time max_time) {
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
            shared_ptr<EventListener> listener = i->second;
            if (!listener->paused()) {
                if (listener->HandleEvent(event)) {
                    ++i;
                } else {
                    listeners_.erase(i++);
                }
            }
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
    return !type.str().empty();
}

bool EventDispatcher::IsListerningType(const EventType& type) const {
    return (listeners_.find(type) != listeners_.end());
}

}
