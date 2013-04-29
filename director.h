#ifndef __kawaii__director__
#define __kawaii__director__

#include "types.h"
#include "actor.h"
#include "event_dispatcher.h"
#include "process/process_scheduler.h"

#include <vector>
#include <memory>
#include <unordered_map>

#include <OpenGLES/ES2/gl.h>

namespace kawaii {
using namespace std;

class Scene;
class Actor;
    
class Director : public enable_shared_from_this<Director> {
public:
    Director()
        : scheduler_(new ProcessScheduler),
          event_dispatcher_(new EventDispatcher),
          display_stats_(true),
          debug_level_(0) {
    }

    virtual ~Director() {}
    virtual bool Init() = 0;
    virtual bool End() { return true; }

    // virtual void Update(const ii_time delta_time) = 0;
    virtual bool HandleEvent(shared_ptr<Event> e) = 0;
    
    ProcessScheduler& scheduler() {
        return *scheduler_;
    }

    EventDispatcher& dispatcher() {
        return *event_dispatcher_;
    }

    void ReshapeProjection();
    void MainLoop(const ii_time delta_time);

    void RunWithScene(shared_ptr<Scene> scene);
    // void ReplaceScene(shared_ptr<Scene> scene);
    // void PushScene(shared_ptr<Scene> scene);
    // void PopScene();

    // void SetNextScene(shared_ptr<Scene> scene);
    shared_ptr<Scene> scene() {
        return running_scene_;
    }
    
    void debug_off() {
        debug_level_ = 0;
    }

    void debug_on(int debug_level) {
        debug_level_ = debug_level;
    }

    int debug_level() {
        return debug_level_;
    }

private:
    Director(const Director&);
    Director& operator=(const Director&);

    void ShowStats();

    unique_ptr<ProcessScheduler> scheduler_;
    unique_ptr<EventDispatcher> event_dispatcher_;
    vector<shared_ptr<Scene> > scene_stack_;

    // scene staff
    shared_ptr<Scene> running_scene_;
    shared_ptr<Scene> next_scene_;

    // stats
    bool display_stats_;
    unsigned int frames_;
    unsigned int total_frames_;
    ii_time accum_dt_;
    float frame_rate_;

    int debug_level_;
};

}

#endif /* defined(__kawaii__director__) */
