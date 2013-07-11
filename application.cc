#include "skidarake/application.h"

#include "skidarake/logger.h"
#include "skidarake/matrix_stack.h"

#include "skidarake/OpenGL_Internal.h"

namespace skidarake {

bool Application::Init(ApplicationComponentFactory& factory) {
    loader_.reset(factory.CreateAssetsLoader());

    director_.reset(factory.CreateDirector());
    if (!director_->Init()) {
        IIERROR("Director::Init() failed.");
        return false;
    }

    return true;
}

void Application::Resize(const float point_width, const float point_height) {
    size_in_points_ = vec2(point_width, point_height);
    size_in_pixels_ = size_in_points_ * content_scale_factor_;
    director_->ReshapeProjection();
}

} // namespace skidarake
