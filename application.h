#ifndef __kawaii__application__
#define __kawaii__application__

#include "vector.h"
#include "vertices.h"

#include <memory>
#include <OpenGLES/ES2/gl.h>

namespace kawaii {
using namespace std;

class Assets;
class Director;

class Application {
public:
    static inline Application *Current() {
        if (__current == NULL) {
            __current = new Application();
            // __current->Initialize();
        }
        return __current;
    }

    static Application *Current();

    ~Application();

    bool Initialize(Assets *assets);
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

    Assets *assets() {
        return assets_;
    }

    Director *director() const {
        return director_;
    }

private:
    static Application *__current;

    Application()
        : total_time_(0),
          size_in_points_(0, 0),
          size_in_pixels_(0, 0),
          content_scale_factor_(1) {
        set_animation_interval(1.0 / 60);
    };
    
    vec2 size_in_points_;
    vec2 size_in_pixels_;
    float content_scale_factor_;

    double animation_interval_;
    float total_time_;

    Assets *assets_;
    Director *director_;
};

} // namespace kawaii

#endif /* defined(__kawaii__application__) */
