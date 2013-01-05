#ifndef __kawaii__director__
#define __kawaii__director__

#include "vector.h"
#include "vertices.h"
#include "actor.h"

#include <vector>
#include <memory>
#include <map>

#include <OpenGLES/ES2/gl.h>

namespace kawaii {
using namespace std;

class Texture2D;
class EventManager;
class ProcessManager;
class Scene;

typedef map<actor_id, shared_ptr<Actor> > ActorTable;

class Director {
public:
    Director();
    ~Director();
    bool Initialize();
    void ReshapeProjection();
    void MainLoop(const float delta_time);

    void AddActor(const shared_ptr<Actor>& actor);
    const shared_ptr<Actor> ActorById(const actor_id id);

    void RunWithScene(shared_ptr<Scene> scene);
    // void ReplaceScene(shared_ptr<Scene> scene);
    // void PushScene(shared_ptr<Scene> scene);
    // void PopScene();

    // void SetNextScene(shared_ptr<Scene> scene);

    void End();

private:
    ActorTable actor_table_;
    EventManager *event_manager_;
    ProcessManager *process_manager_;
    vector<shared_ptr<Scene> > scene_stack_;
    shared_ptr<Scene> running_scene_;
    shared_ptr<Scene> next_scene_;
};

}

#endif /* defined(__kawaii__director__) */
