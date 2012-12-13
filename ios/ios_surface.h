#ifndef __kawaii__ios_surface__
#define __kawaii__ios_surface__

#include "surface.h"

@class EAGLView;

namespace kawaii {

class IOSSurface : public Surface {
public:
    IOSSurface(EAGLView *eagl_view) : eagl_view_(eagl_view) {}
    const float scale() const;
    const vec2 size_in_points() const;
    const vec2 size_in_pixels() const;
    // void SwapBuffers();

private:
    EAGLView *eagl_view_;
};
    
}

#endif /* defined(__kawaii__ios_surface__) */
