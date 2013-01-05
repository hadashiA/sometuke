#include "application.h"

#include "logger.h"
#include "matrix_stack.h"
#include "director.h"

#include "OpenGL_Internal.h"

namespace kawaii {

Application *Application::__current = NULL;
Application *Application::Current() {
    if (__current == NULL) {
        __current = new Application();
        // __current->Initialize();
    }
    return __current;
}

Application::~Application() {
    delete director_;
}

bool Application::Initialize(Assets *assets) {
    assets_ = assets;
    if (assets_ == NULL) {
        return false;
    }

    director_ = new Director;
    if (!director_->Initialize()) {
        return false;
    };

    return true;
}

void Application::Resize(const float point_width, const float point_height) {
    size_in_points_ = vec2(point_width, point_height);
    size_in_pixels_ = size_in_points_ * content_scale_factor_;
    director_->ReshapeProjection();
}

} // namespace kawaii
