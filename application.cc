#include "application.h"

#include "logger.h"
#include "matrix_stack.h"

#include "OpenGL_Internal.h"

namespace kawaii {

bool Application::Init(Director *director, Assets *assets) {
    assets_.reset(assets);
    if (!assets_) {
        return false;
    }

    director_.reset(director);
    if (director_->Init()) {
        return false;
    }

    return true;
}

void Application::Resize(const float point_width, const float point_height) {
    size_in_points_ = vec2(point_width, point_height);
    size_in_pixels_ = size_in_points_ * content_scale_factor_;
    director_->ReshapeProjection();
}

} // namespace kawaii
