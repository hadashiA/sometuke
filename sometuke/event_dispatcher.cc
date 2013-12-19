#include "sometuke/event_dispatcher.h"

#include "sometuke/director.h"
#include "sometuke/logger.h"

#include <functional>
#include <cassert>

namespace sometuke {
using namespace std;

// EventEmitter

void EventEmitter::On(const EventType& type,
                      const client_id sender_id,
                      EventCallback callback) {
    Director::Instance().dispatcher().On(type, sender_id, cid_, callback);
}

void EventEmitter::Off() {
    Director::Instance().dispatcher().Off(cid_);
}

void EventEmitter::Trigger(const shared_ptr<Event>& event) {
    Director::Instance().dispatcher().Trigger(event, cid_);
}

// EventDispatcher

void EventDispatcher::On(const EventType& type,
                         const client_id sender_id,
                         const client_id receiver_id,
                         const EventCallback& callback) {
    
    EventHandler handler(sender_id, receiver_id, callback);
    handlers_.emplace(type, handler);
}

void EventDispatcher::Off(const EventType& type) {
    auto range = handlers_.equal_range(type);
    handlers_.erase(range.first, range.second);
}

bool EventDispatcher::Off(const client_id receiver_id) {
    for (auto i = handlers_.begin(); i != handlers_.end();) {
        EventHandler handler = i->second;

        if (receiver_id == handler.receiver_id) {
            handlers_.erase(i++);
        } else {
            ++i;
        }
    }
}

void EventDispatcher::Off(const client_id receiver_id, const EventType& type) {
    auto range = handlers_.equal_range(type);
    for (auto i = range.first; i != range.second;) {
        EventHandler handler = i->second;

        if (listener == handler.listener.lock()) {
            handlers_.erase(i++);
        } else {
            ++i;
        }
    }
}

void EventDispatcher::QuickTrigger(const shared_ptr<Event>& event) {
    const EventType& type = event->type;

    // if (!IsValidType(type) || !IsListerningType(type)) {
    if (!IsValidType(type)) {
        return false;
    }

    bool emitted = false;

    auto range = handlers_.equal_range(type);
    for (auto i = range.first; i != range.second;) {
        const EventHandler& handler = i->second;
        handler.callback(event);
    }
}

void EventDispatcher::Trigger(const shared_ptr<Event>& event) {
    if (event->quick) {
        QuickTrigger(event);
        return;
     }

    assert(active_queue_index_ >= 0);
    assert(active_queue_index_ < NUM_QUEUES);

    if (!IsValidType(event->type)) {
        return;
    }

    queues_[active_queue_index_].push_back(event);
}

void EventDispatcher::Tick(const s2_time max_time) {
    std::list<shared_ptr<Event> > queue = queues_[active_queue_index_];

    // swap active queues, make sure new queue is empty after the swap..
    active_queue_index_ = (active_queue_index_ + 1) % NUM_QUEUES;
    queues_[active_queue_index_].clear();

    while (!queue.empty()) {
        const shared_ptr<Event>& event = queue.front();
        queue.pop_front();

        auto range = handlers_.equal_range(event->type);
        for (auto i = range.first; i != range.second;) {
            const EventHandler& handler = i->second;
            
            handler.callback(event);
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
}

bool EventDispatcher::IsValidType(const EventType& type) const {
    return !type.empty();
}

}
