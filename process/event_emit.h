#ifndef __kawaii__process_event_emit__
#define __kawaii__process_event_emit__

#include "kawaii/process/process.h"
#include "kawaii/event_dispatcher.h"
#include "kawaii/application.h"

namespace kawaii {

class EventEmit : public Process {
public:
    static const HashedString TYPE;

    EventEmit(shared_ptr<Event> event)
        : event_(event){}

    const HashedString& type() const {
        return TYPE;
    }

    virtual bool Step(const ii_time delta) {
        Application::Instance().dispatcher().Queue(event_);
        return false;
    }

private:
    shared_ptr<Event> event_;
};

}

#endif /* defined(__kawaii__process_event_emit__) */
