#include "sometuke/event_dispatcher.h"

#include "sometuke/director.h"
#include "sometuke/logger.h"

#include <functional>
#include <cassert>

namespace sometuke {

bool EventListener::Off() {
    return Director::Instance().dispatcher().Off(shared_from_this());
}

void EventListener::On(const EventType& type, EventCallback callback) {
    EventHandler handler;
    handler.callback = callback;
    handler.listener = shared_from_this();

    Director::Instance().dispatcher().On(type, handler);
}

void EventDispatcher::On(const EventType& type, EventHandler handler) {
    handlers_.emplace(std::make_pair(type, handler));
}

bool EventDispatcher::Off(const EventType& type) {
    auto range = handlers_.equal_range(type);
    handlers_.erase(range.first, range.second);

    return true;
}

bool EventDispatcher::Off(shared_ptr<EventListener> listener) {
    for (auto i = handlers_.begin(); i != handlers_.end();) {
        EventHandler handler = i->second;

        if (listener == handler.listener.lock()) {
            handlers_.erase(i++);
        } else {
            ++i;
        }
    }
    return true;
}

bool EventDispatcher::Off(const EventType& type, shared_ptr<EventListener> listener) {
    auto range = handlers_.equal_range(type);
    for (auto i = range.first; i != range.second;) {
        EventHandler handler = i->second;

        if (listener == handler.listener.lock()) {
            handlers_.erase(i++);
        } else {
            ++i;
        }
    }
    return true;
}

bool EventDispatcher::Trigger(const shared_ptr<Event>& event) {
    const EventType& type = event->type;

    // if (!IsValidType(type) || !IsListerningType(type)) {
    if (!IsValidType(type)) {
        return false;
    }

    bool emitted = false;

    auto range = handlers_.equal_range(type);
    for (auto i = range.first; i != range.second;) {
        EventHandler handler = i->second;
        
        if (const shared_ptr<EventListener>& listener = handler.listener.lock()) {
            emitted = true;
            handler.callback(event);
            ++i;
        } else {
            handlers_.erase(i++);
        }
    }

    return emitted;
}

bool EventDispatcher::Queue(const shared_ptr<Event>& event) {
    assert(active_queue_index_ >= 0);
    assert(active_queue_index_ < NUM_QUEUES);

    if (!IsValidType(event->type)) {
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

        auto range = handlers_.equal_range(event->type);
        for (auto i = range.first; i != range.second;) {
            EventHandler handler = i->second;
            
            if (const shared_ptr<EventListener>& listener = handler.listener.lock()) {
                handler.callback(event);
                ++i;
            } else {
                handlers_.erase(i++);
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
    return !type.empty();
}

}
