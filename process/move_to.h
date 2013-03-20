#ifndef __kawaii__process_move_to__
#define __kawaii__process_move_to__

#include "process.h"

#include "actor.h"

namespace kawaii {

class MoveTo : public Process {
public:
    MoveTo(actor_id id, const vec3 to, const ii_time duration)

private:
    vec3 from_;
    vec3 to_;
    vec3 delta_;
};

}

#endif /* defined(__kawaii__process_move_to__) */
