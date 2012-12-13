#include "ios_surface.h"

#include "EAGLView.h"

namespace kawaii {

const float IOSSurface::scale() const {
    return [UIScreen mainScreen].scale;
}

const vec2 IOSSurface::size_in_points() const {
    return vec2(eagl_view_.backingWidth,
                eagl_view_.backingHeight);
}

const vec2 IOSSurface::size_in_pixels() const {
    return size_in_points() * content_scale_factor();
}

// void IOSSurface::SwapBuffers() {
//     [eagl_view_ swapBuffers];
// }

}
