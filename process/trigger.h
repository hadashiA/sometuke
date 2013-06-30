#ifndef __kawaii__process_trigger__
#define __kawaii__process_trigger__

#include "kawaii/process/process.h"
#include "kawaii/event_dispatcher.h"
#include "kawaii/application.h"

namespace kawaii {

class Trigger : public Process {
public:
    static const HashedString TYPE;

    Trigger(shared_ptr<Event> event)
        : event_(event){}

    const HashedString& type() const {
        return TYPE;
    }

    virtual bool Update(const ii_time delta) {
        Application::Instance().dispatcher().Queue(event_);
        return false;
    }

private:
    shared_ptr<Event> event_;
};

}

#endif /* defined(__kawaii__process_event_emit__) */
