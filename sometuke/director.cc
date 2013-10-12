#include "sometuke/director.h"

#include "sometuke/director.h"
#include "sometuke/event_dispatcher.h"
#include "sometuke/matrix_stack.h"
#include "sometuke/logger.h"
#include "sometuke/scene.h"
#include "sometuke/node/label_atlas.h"

#include "sometuke/OpenGL_Internal.h"

#include <sstream>

namespace sometuke {

void Director::Resize(const float point_width, const float point_height) {
    size_in_points_ = vec2(point_width, point_height);
    size_in_pixels_ = size_in_points_ * content_scale_factor_;
    ReshapeProjection();
}

void Director::Pause() {
    if (paused_) {
        return;
    }

    animation_interval_was_ = animation_interval_;
    // animation_interval_ = 1 / 4.0; // when paused, don't consume CPU

    paused_ = true;
}

void Director::Resume() {
    if (!paused_) {
        return;
    }

    animation_interval_ = animation_interval_was_;
    
    paused_ = false;
    NextDeltaTimeZero();
}

void Director::MainLoop(const s2_time delta_time) {
    if (next_delta_time_zero_) {
        dt_ = 0;
        next_delta_time_zero_ = false;
    } else {
        dt_ = delta_time;
    }

    event_dispatcher_->Tick(0.02);
    if (!paused_) {
        scheduler_->Update(dt_);
        process_manager_->Update(dt_);
    }

    if (next_scene_) {
        EnterNextScene();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MatrixStack<GLModelView>::Instance().Push();

    if (running_scene_) {
        running_scene_->Visit();
    }

    MatrixStack<GLModelView>::Instance().Pop();

    if (stats_shown_) {
        ShowStats();
    }

    total_frames_++;
}

void Director::RunWithScene(const shared_ptr<Scene>& scene) {
    PushScene(scene);
}

void Director::ReplaceScene(const shared_ptr<Scene>& scene) {
    scene_stack_.pop();
    scene_stack_.push(scene);
    next_scene_ = scene;
}

void Director::PushScene(const shared_ptr<Scene>& scene) {
    send_cleanup_to_scene_ = false;

    scene_stack_.push(scene);
    next_scene_ = scene;
}

void Director::PopScene() {
    scene_stack_.pop();
    if (!scene_stack_.empty()) {
        send_cleanup_to_scene_ = true;
        next_scene_ = scene_stack_.top();
    }
}

// private

void Director::EnterNextScene() {
    if (running_scene_) {
        running_scene_->OnExit();
        if (send_cleanup_to_scene_) {
            running_scene_->Cleanup();
        }
    }

    running_scene_ = next_scene_;
    next_scene_.reset();

    running_scene_->OnEnter();
}

bool Director::CreateStatsLabel() {
    fps_label_ = make_shared<LabelAtlas>();
    if (!fps_label_->InitWithText("00.0", "fps_images.png", 12, 32, '.')) {
        S2ERROR("Faild init fps_label_.");
        return false;
    }

    fps_label_->set_position(0, 0);
    return true;
}

void Director::ShowStats() {
    stringstream ss;

    frames_++;
    accum_dt_ += dt_;

    if (accum_dt_ > stats_interval_) {
        frame_rate_ = frames_ / accum_dt_;
        frames_ = 0;
        accum_dt_ = 0;

        ss << frame_rate_;
        fps_label_->set_text(ss.str());
    }

    fps_label_->Visit();
}

void Director::ReshapeProjection() {
    glViewport(0, 0, size_in_points_.x, size_in_points_.y);

    switch (projection_type_) {
    case ProjectionType::ORTHOGONAL: {
        mat4& projection = MatrixStack<GLProjection>::Instance().Push();
        projection = mat4::OrthographicProjection(0, size_in_points_.x,
                                                  0, size_in_points_.y,
                                                  -1024, 1024);
        MatrixStack<GLModelView>::Instance().Push();
        break;
    }
    case ProjectionType::PERSPECTIVE: {
        mat4& projection = MatrixStack<GLProjection>::Instance().Push();
        float zeye = size_in_pixels_.y / 1.1566f / content_scale_factor_;
        projection = mat4::Perspective(60, size_in_points_.x / size_in_points_.y,
                                       0.1f, zeye * 2);
        
        mat4& model_view = MatrixStack<GLModelView>::Instance().Push();
        vec3 eye(size_in_points_.x / 2, size_in_points_.y / 2, zeye);
        vec3 center(size_in_points_.x / 2, size_in_points_.y / 2, 0);
        vec3 up(0, 1, 0);
        model_view = mat4::LookAt(eye, center, up);
        break;
    }
    default:
        S2ERROR("Unrecognized projection");
        break;
    }

    CHECK_GL_ERROR();
}

bool Director::InitGL() {
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

}
