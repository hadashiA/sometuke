#ifndef __sometuke__director__
#define __sometuke__director__

#include "sometuke/types.h"
#include "sometuke/vector.h"
#include "sometuke/scheduler.h"
#include "sometuke/process/process_manager.h"
#include "sometuke/event_dispatcher.h"
#include "sometuke/application_component.h"

#include <vector>
#include <memory>

#include <OpenGLES/ES2/gl.h>

namespace sometuke {
using namespace std;

class Scene;
class LabelAtlas;
    
class Director : public enable_shared_from_this<Director> {
public:
    static Director& Instance() {
        static unique_ptr<Director> __instance(new Director);
        return *__instance;
    }
        
    virtual ~Director() {}


    template <typename ComponentFactory>
    bool Init() {
        ComponentFactory factory;
        loader_.reset(factory.CreateAssetsLoader());
        system_fonts_.reset(factory.CreateSystemFontLoader());
        
        return InitGL();
    }
    
    Scheduler& scheduler() {
        return *scheduler_;
    }

    ProcessManager& process() {
        return *process_manager_;
    }

    EventDispatcher& dispatcher() {
        return *event_dispatcher_;
    }

    AssetsLoader& loader() const {
        return *loader_;
    }

    SystemFontLoader& system_fonts() {
        return *system_fonts_;
    }

    void Resize(const float point_width, const float point_height);

    const vec2& size_in_points() {
        return size_in_points_;
    }

    const vec2& size_in_pixels() {
        return size_in_pixels_;
    }

    const float content_scale_factor() {
        return content_scale_factor_;
    }

    void set_content_scale_factor(const float value) {
        content_scale_factor_ = value;
    }

    const double animation_interval() {
        return animation_interval_;
    }

    void set_animation_interval(const double value) {
        animation_interval_ = value;
    }

    void MainLoop(const s2_time delta_time);

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

    void set_stats_interval(s2_time value) {
        stats_interval_ = value;
    }

private:
    Director()
        : scheduler_(new Scheduler),
          process_manager_(new ProcessManager),
          event_dispatcher_(new EventDispatcher),
          frames_(0),
          total_frames_(0),
          total_time_(0),
          size_in_points_(0, 0),
          size_in_pixels_(0, 0),
          content_scale_factor_(1),
          stats_shown_(true),
          stats_interval_(0.1),
          debug_drawing_(false) {

        set_animation_interval(1.0 / 60);
    }
    Director& operator=(const Director&);

    bool InitGL();
    bool CreateStatsLabel();
    void ReshapeProjection();
    void ShowStats(const s2_time delta);

    vec2 size_in_points_;
    vec2 size_in_pixels_;
    float content_scale_factor_;

    double animation_interval_;
    double total_time_;

    unique_ptr<Scheduler> scheduler_;
    unique_ptr<ProcessManager> process_manager_;
    unique_ptr<EventDispatcher> event_dispatcher_;
    unique_ptr<AssetsLoader> loader_;
    unique_ptr<SystemFontLoader> system_fonts_;

    // scene stuff
    vector<shared_ptr<Scene> > scene_stack_;
    shared_ptr<Scene> running_scene_;
    shared_ptr<Scene> next_scene_;

    // stats
    unsigned int frames_;
    unsigned int total_frames_;
    s2_time accum_dt_;
    float frame_rate_;

    bool stats_shown_;
    s2_time stats_interval_;
    bool debug_drawing_;

    shared_ptr<LabelAtlas> fps_label_;
};

}

#endif /* defined(__sometuke__director__) */
