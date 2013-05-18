#include "kawaii/director.h"

#include "kawaii/application.h"
#include "kawaii/event_dispatcher.h"
#include "kawaii/matrix_stack.h"
#include "kawaii/logger.h"
#include "kawaii/node/scene.h"
#include "kawaii/node/label_atlas.h"

#include "kawaii/OpenGL_Internal.h"

namespace kawaii {

bool Director::Init() {
    fps_label_ = make_shared<LabelAtlas>();
    fps_label_->InitWithText("00.0", "fps_images.png", 12, 32, '.');

    return true;
}

void Director::ReshapeProjection() {
    Application& app = Application::Instance();
    const vec2 size_in_points = app.size_in_points();
    const vec2 size_in_pixels = app.size_in_pixels();
    const float content_scale_factor = app.content_scale_factor();
    
    glViewport(0, 0, size_in_points.x, size_in_points.y);

    mat4& projection = MatrixStack<GLProjection>::Instance().Push();
    float zeye = size_in_pixels.y / 1.1566f / content_scale_factor;
    projection = mat4::Perspective(60, size_in_points.x / size_in_points.y,
                                   0.1f, zeye * 2);

    mat4& model_view = MatrixStack<GLModelView>::Instance().Push();
    vec3 eye(size_in_points.x / 2, size_in_points.y / 2, zeye);
    vec3 center(size_in_points.x / 2, size_in_points.y / 2, 0);
    vec3 up(0, 1, 0);
    model_view = mat4::LookAt(eye, center, up);

    CHECK_GL_ERROR();
}

void Director::MainLoop(const ii_time delta_time) {
    event_dispatcher_->Tick(0.02);
    scheduler_->Update(delta_time);
    process_manager_->Update(delta_time);

    if (running_scene_) {
        running_scene_->Visit();
    }

    ShowStats();
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
    fps_label_->Visit();
}

}
