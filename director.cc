#include "director.h"

#include "application.h"
#include "assets.h"
#include "shader_cache.h"
#include "texture_2d.h"
#include "process_scheduler.h"
#include "event_dispatcher.h"
#include "matrix_stack.h"
#include "logger.h"
#include "scene.h"

#include "OpenGL_Internal.h"

namespace kawaii {

void Director::ReshapeProjection() {
    unique_ptr<Application>& app = Application::Current();
    const vec2 size_in_points = app->size_in_points();
    const vec2 size_in_pixels = app->size_in_pixels();
    const float content_scale_factor = app->content_scale_factor();
    
    glViewport(0, 0, size_in_points.x, size_in_points.y);

    mat4& projection = MatrixStack::GLProjection()->Push();
    float zeye = size_in_pixels.y / 1.1566f / content_scale_factor;
    projection = mat4::Perspective(60, size_in_points.x / size_in_points.y,
                                   0.1f, zeye * 2);

    mat4& model_view = MatrixStack::GLModelView()->Push();
    vec3 eye(size_in_points.x / 2, size_in_points.y / 2, zeye);
    vec3 center(size_in_points.x / 2, size_in_points.y / 2, 0);
    vec3 up(0, 1, 0);
    model_view = mat4::LookAt(eye, center, up);

    CHECK_GL_ERROR();
}

void Director::MainLoop(const ii_time delta_time) {
    event_dispatcher_->Tick(0.02);
    scheduler_->Update(delta_time);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MatrixStack::GLModelView()->Push();

    if (running_scene_) {
        running_scene_->Render();
    }

    if (display_stats_) {
        ShowStats();
    }

    MatrixStack::GLModelView()->Pop();
}

void Director::RunWithScene(shared_ptr<Scene> scene) {
    running_scene_ = scene;
    running_scene_->OnEnter();
}

// void Director::ReplaceScene(shared_ptr<Scene> scene) {
// }

// void Director::PushScene(shared_ptr<Scene> scene) {
// }

// void Director::PopScene() {
// }

// void Director::SetNextScene(shared_ptr<Scene> scene) {
// }

// private

void Director::ShowStats() {
}

}
