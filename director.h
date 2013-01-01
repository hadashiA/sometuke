#ifndef __kawaii__director__
#define __kawaii__director__

#include "vertices.h"
#include "actor.h"
#include "event_manager.h"
#include "process_manager.h"

#include <memory>
#include <map>

#include <OpenGLES/ES2/gl.h>

namespace kawaii {
using namespace std;

typedef map<actor_id, shared_ptr<Actor> > ActorTable;

class Texture2D;

class Director {
public:
    bool Initialize();
    void Update(float delta_time);

private:
    ActorTable actor_table_;
    EventManager *event_manager_;
    ProcessManager *process_manager_;

    // debug
    shared_ptr<Texture2D> texture_;
    GLenum blend_func_src_;
    GLenum blend_func_dst_;
    Quad_P3F_C4B_T2F quad_;

};

}

#endif /* defined(__kawaii__director__) */
