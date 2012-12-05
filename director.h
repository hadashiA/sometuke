#ifndef __kawaii__director__
#define __kawaii__director__

#include <OpenGLES/ES2/gl.h>

#include "vector.h"

namespace kawaii {
using namespace std;

class Director {
public:
    explicit Director(float content_scale_factor) :
        content_scale_factor_(content_scale_factor),
        total_time_(0) {
        set_animation_interval(1.0 / 60);
    };
    
    void SetProjection(float width, float height);
    void DrawScene(float deltaTime);
    void Sample();

    const double animation_interval() const {
        return animation_interval_;
    }

    void set_animation_interval(double value) {
        animation_interval_ = value;
    }

private:
    vec2 winSizeInPoints_;
    vec2 winSizeInPixels_;
    float content_scale_factor_;

    double animation_interval_;

    float total_time_;
};

} // namespace kawaii

#endif /* defined(__kawaii__director__) */
