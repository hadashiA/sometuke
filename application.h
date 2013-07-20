#ifndef __sometuke__application__
#define __sometuke__application__

#include "sometuke/vector.h"
#include "sometuke/vertices.h"
#include "sometuke/director.h"
#include "sometuke/assets_loader.h"

#include <memory>
#include <OpenGLES/ES2/gl.h>

namespace sometuke {
using namespace std;

class AssetsLoader;

class ApplicationComponentFactory {
public:
    virtual ~ApplicationComponentFactory() {}

    virtual Director *CreateDirector() = 0;
    virtual AssetsLoader *CreateAssetsLoader() = 0;
};

class Application {
public:
    static Application& Instance() {
        static unique_ptr<Application> __instance(new Application);
        return *__instance;
    }

    bool Init(ApplicationComponentFactory& factory);

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

    AssetsLoader& loader() const {
        return *loader_;
    }

    Director& director() const {
        return *director_;
    }

    Scheduler& scheduler() const {
        return director_->scheduler();
    }

    ProcessManager& process() const {
        return director_->process();
    }

    EventDispatcher& dispatcher() const {
        return director_->dispatcher();
    }

private:
    Application()
        : total_time_(0),
          size_in_points_(0, 0),
          size_in_pixels_(0, 0),
          content_scale_factor_(1) {
        set_animation_interval(1.0 / 60);
    };

    Application(const Application&);
    Application& operator=(const Application&);
    
    vec2 size_in_points_;
    vec2 size_in_pixels_;
    float content_scale_factor_;

    double animation_interval_;
    float total_time_;

    shared_ptr<Director> director_;
    unique_ptr<AssetsLoader> loader_;
};

} // namespace sometuke

#endif /* defined(__sometuke__application__) */
