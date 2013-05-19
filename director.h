#ifndef __kawaii__director__
#define __kawaii__director__

#include "kawaii/types.h"
#include "kawaii/scheduler.h"
#include "kawaii/process/process_manager.h"
#include "kawaii/event_dispatcher.h"

#include <vector>
#include <memory>

#include <OpenGLES/ES2/gl.h>

namespace kawaii {
using namespace std;

class Scene;
class LabelAtlas;
    
class Director : public enable_shared_from_this<Director> {
public:
    Director()
        : scheduler_(new Scheduler),
          process_manager_(new ProcessManager),
          event_dispatcher_(new EventDispatcher),
          frames_(0),
          total_frames_(0),
          stats_shown_(true),
          stats_interval_(0.1),
          debug_drawing_(false) {
    }
        
    virtual ~Director() {}
    virtual bool Init();
    virtual bool End()  { return true; }

    virtual bool HandleEvent(shared_ptr<Event> e) = 0;
    
    Scheduler& scheduler() {
        return *scheduler_;
    }

    ProcessManager& process_manager() {
        return *process_manager_;
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
    
    void stats_off() {
        stats_shown_ = false;
    }

    void stats_on() {
        stats_shown_ = true;
    }

    bool stats_shown() const {
        return stats_shown_;
    }

    bool debug_drawing() const {
        return debug_drawing_;
    }

    void set_stats_interval(ii_time value) {
        stats_interval_ = value;
    }

protected:


private:
    Director(const Director&);
    Director& operator=(const Director&);

    bool CreateStatsLabel();
    void ShowStats(const ii_time delta);
    

    unique_ptr<Scheduler> scheduler_;
    unique_ptr<ProcessManager> process_manager_;
    unique_ptr<EventDispatcher> event_dispatcher_;

    // scene staff
    vector<shared_ptr<Scene> > scene_stack_;
    shared_ptr<Scene> running_scene_;
    shared_ptr<Scene> next_scene_;

    // stats
    unsigned int frames_;
    unsigned int total_frames_;
    ii_time accum_dt_;
    float frame_rate_;

    bool stats_shown_;
    ii_time stats_interval_;
    bool debug_drawing_;

    // debug
    shared_ptr<LabelAtlas> fps_label_;
};

}

#endif /* defined(__kawaii__director__) */
