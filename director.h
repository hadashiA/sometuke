#ifndef __kawaii__director__
#define __kawaii__director__

#include <memory>
#include <OpenGLES/ES2/gl.h>

#include "vector.h"
#include "surface.h"

namespace kawaii {
using namespace std;

class Director {
public:
    static Director *Current();

    void ReshapeProjection();
    void MainLoop(float deltaTime);

    const Surface *surface() {
        return surface_.get();
    }

    void set_surface(Surface *surface) {
        surface_.reset(surface);
    }

    const double animation_interval() const {
        return animation_interval_;
    }

    void set_animation_interval(double value) {
        animation_interval_ = value;
    }

private:
    static Director *__current;

    Director() :
        total_time_(0) {
        set_animation_interval(1.0 / 60);
    };
    
    shared_ptr<Surface> surface_;

    double animation_interval_;
    float total_time_;
};

} // namespace kawaii

#endif /* defined(__kawaii__director__) */
