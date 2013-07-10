#ifndef __skidarak__process_trigger__
#define __skidarak__process_trigger__

#include "skidarak/process/process.h"
#include "skidarak/event_dispatcher.h"
#include "skidarak/application.h"

namespace skidarak {

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

#endif /* defined(__skidarak__process_event_emit__) */
