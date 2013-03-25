#ifndef __kawaii__process_move_to__
#define __kawaii__process_move_to__

#include "interval.h"

#include "application.h"
#include "logger.h"

namespace kawaii {

class MoveTo : public Interval {
public:
    MoveTo(ActorId id, const vec3 to, const ii_time duration)
        : Interval(duration),
          ActorId_(id),
          to_(to) {
        OnEnter();
    }

    virtual ~MoveTo() {}

    virtual void OnEnter() {
        //shared_ptr<Actor> actor = Application::Instance().director()[ActorId_];
        //if (actor) {
            // from_  = actor->position;
            // delta_ = to_ - from_;
        //}
    }

    virtual bool Update(const float progress) {
        //Director& director = Application::Instance().director();
        //shared_ptr<Actor> actor = director.ActorForId(ActorId_);
        //if (actor) {
        //    vec3 pos = from_ + (delta_ * progress);
        //    IIINFO("%s", IIINSPECT(pos));
        //} else {
            return false;
        //}
    }

private:
    ActorId ActorId_;
    vec3 from_;
    vec3 to_;
    vec3 delta_;
};

}

#endif /* defined(__kawaii__process_move_to__) */
