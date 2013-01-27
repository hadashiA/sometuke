#ifndef __kawaii__director__
#define __kawaii__director__

#include "types.h"

#include <vector>
#include <memory>
#include <map>

#include <OpenGLES/ES2/gl.h>

namespace kawaii {
using namespace std;

class ProcessScheduler;
class EventDispatcher;
class Scene;

class Director {
public:
    Director();
    ~Director();

    ProcessScheduler *scheduler() const {
        return scheduler_;
    }

    EventDispatcher *dispatcher() const {
        return event_dispatcher_;
    }

    void ReshapeProjection();
    void MainLoop(const ii_time delta_time);

    void RunWithScene(shared_ptr<Scene> scene);
    // void ReplaceScene(shared_ptr<Scene> scene);
    // void PushScene(shared_ptr<Scene> scene);
    // void PopScene();

    // void SetNextScene(shared_ptr<Scene> scene);

    void End();

private:
    void ShowStats();

    ProcessScheduler *scheduler_;
    EventDispatcher *event_dispatcher_;
    vector<shared_ptr<Scene> > scene_stack_;
    shared_ptr<Scene> running_scene_;
    shared_ptr<Scene> next_scene_;
    bool display_stats_;
};

}

#endif /* defined(__kawaii__director__) */
