#ifndef __kawaii__surface__
#define __kawaii__surface__

#include "vector.h"

namespace kawaii {

class Surface {
public:
    virtual ~Surface() {}
    virtual const float content_scale_factor() const = 0;
    virtual const vec2 size_in_points() const = 0;
    virtual const vec2 size_in_pixels() const = 0;
    // virtual void SwapBuffers() = 0;
};
    
}

#endif /* defined(__kawaii__surface__) */
