#include "sometuke/director.h"

#include "sometuke/application.h"
#include "sometuke/event_dispatcher.h"
#include "sometuke/matrix_stack.h"
#include "sometuke/logger.h"
#include "sometuke/scene.h"
#include "sometuke/node/label_atlas.h"

#include "sometuke/OpenGL_Internal.h"

#include <sstream>

namespace sometuke {

bool Director::Init() {
    glClearColor(0.5, 0.5, 0.5, 1);

    glClearDepthf(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);

    CHECK_GL_ERROR_DEBUG();

    if (!CreateStatsLabel()) {
        S2ERROR("Fails CreateStatsLabel()");
        return false;
    }

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

void Director::MainLoop(const s2_time delta_time) {
    event_dispatcher_->Tick(0.02);
    scheduler_->Update(delta_time);
    process_manager_->Update(delta_time);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MatrixStack<GLModelView>::Instance().Push();

    if (running_scene_) {
        running_scene_->Visit();
    }

    MatrixStack<GLModelView>::Instance().Pop();

    if (stats_shown_) {
        ShowStats(delta_time);
    }

    total_frames_++;
}

void Director::RunWithScene(shared_ptr<Scene> scene) {
    running_scene_ = scene;
    running_scene_->Enter();
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

bool Director::CreateStatsLabel() {
    fps_label_ = make_shared<LabelAtlas>();
    if (!fps_label_->InitWithText("00.0", "fps_images.png", 12, 32, '.')) {
        S2ERROR("Faild init fps_label_.");
        return false;
    }

    fps_label_->set_position(0, 0);
    return true;
}

void Director::ShowStats(const s2_time delta) {
    stringstream ss;

    frames_++;
    accum_dt_ += delta;

    if (accum_dt_ > stats_interval_) {
        frame_rate_ = frames_ / accum_dt_;
        frames_ = 0;
        accum_dt_ = 0;

        ss << frame_rate_;
        fps_label_->set_text(ss.str());
    }

    fps_label_->Visit();
}

}
