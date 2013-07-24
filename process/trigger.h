#ifndef __sometuke__process_trigger__
#define __sometuke__process_trigger__

#include "sometuke/process/process.h"
#include "sometuke/event_dispatcher.h"
#include "sometuke/director.h"

namespace sometuke {

class Trigger : public Process {
public:
    static const HashedString TYPE;

    Trigger(shared_ptr<Event> event)
        : event_(event){}

    const HashedString& type() const {
        return TYPE;
    }

    virtual bool Update(const s2_time delta) {
        Director::Instance().dispatcher().Queue(event_);
        return false;
    }

private:
    shared_ptr<Event> event_;
};

}

#endif /* defined(__sometuke__process_event_emit__) */
