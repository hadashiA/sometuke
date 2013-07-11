#ifndef __skidarake__process_trigger__
#define __skidarake__process_trigger__

#include "skidarake/process/process.h"
#include "skidarake/event_dispatcher.h"
#include "skidarake/application.h"

namespace skidarake {

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

#endif /* defined(__skidarake__process_event_emit__) */
