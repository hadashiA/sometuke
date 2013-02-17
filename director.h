#ifndef __kawaii__director__
#define __kawaii__director__

#include "types.h"
#include "process_scheduler.h"
#include "event_dispatcher.h"

#include <vector>
#include <memory>
#include <map>

#include <OpenGLES/ES2/gl.h>

namespace kawaii {
using namespace std;

class Scene;

class Director {
public:
    Director()
        : scheduler_(new ProcessScheduler),
          event_dispatcher_(new EventDispatcher),
          display_stats_(true) {
    }

    unique_ptr<ProcessScheduler>& scheduler() {
        return scheduler_;
    }

    unique_ptr<EventDispatcher>& dispatcher() {
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

    unique_ptr<ProcessScheduler> scheduler_;
    unique_ptr<EventDispatcher> event_dispatcher_;
    vector<shared_ptr<Scene> > scene_stack_;
    shared_ptr<Scene> running_scene_;
    shared_ptr<Scene> next_scene_;

    // stats
    bool display_stats_;
    unsigned int frames_;
    unsigned int total_frames_;
    ii_time accum_dt_;
    float frame_rate_;
};

}

#endif /* defined(__kawaii__director__) */
