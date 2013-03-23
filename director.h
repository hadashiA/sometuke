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
    
typedef unordered_map<actor_id, shared_ptr<Actor> > ActorTable;

class Director : public enable_shared_from_this<Director> {
public:
    Director()
        : scheduler_(new ProcessScheduler),
          event_dispatcher_(new EventDispatcher),
          display_stats_(true) {
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

    void AddActor(shared_ptr<Actor> actor) {
        actor_id id = actor->id();
        actor_table_[id] = actor;
    }
    
    const shared_ptr<Actor> ActorById(const actor_id id) {
        return actor_table_[id];
    }

private:
    void ShowStats();

    unique_ptr<ProcessScheduler> scheduler_;
    unique_ptr<EventDispatcher> event_dispatcher_;
    vector<shared_ptr<Scene> > scene_stack_;

    // scene staff
    shared_ptr<Scene> running_scene_;
    shared_ptr<Scene> next_scene_;

    // actor staff
    ActorTable actor_table_;
    unordered_map<actor_id, shared_ptr<Node> > node_for_actor_id_;

    // stats
    bool display_stats_;
    unsigned int frames_;
    unsigned int total_frames_;
    ii_time accum_dt_;
    float frame_rate_;
};

}

#endif /* defined(__kawaii__director__) */
