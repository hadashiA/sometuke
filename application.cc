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

bool Application::Initialize() {
    director_ = new Director;
    if (!director_->Initialize()) {
        return false;
    };

    return true;
}

void Application::ReshapeProjection(const float width, const float height) {
    const vec2 size_in_points(width, height);
    const vec2 size_in_pixels = size_in_points * content_scale_factor_;
    
    IIINFO("SetProjection %fx%f", size_in_points.x, size_in_points.y);
    glViewport(0, 0, size_in_points.x, size_in_points.y);

    float zeye = size_in_pixels.y / 1.1566f / content_scale_factor_;
    mat4 projection = mat4::Perspective(60, size_in_points.x / size_in_points.y,
                                        0.1f, zeye * 2);
    MatrixStack::GLProjection()->Push(projection);

    vec3 eye(size_in_points.x / 2, size_in_points.y / 2, zeye);
    vec3 center(size_in_points.x / 2, size_in_points.y / 2, 0);
    vec3 up(0, 1, 0);
    mat4 model_view = mat4::LookAt(eye, center, up);
    MatrixStack::GLModelView()->Push(model_view);

    CHECK_GL_ERROR();
}

void Application::MainLoop(float delta_time) {
    total_time_ += delta_time;
    director_->Update(delta_time);
}

} // namespace kawaii
