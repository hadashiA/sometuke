#include "node.h"

#include "matrix_stack.h"
#include "application.h"
#include "logger.h"

#include <cassert>

namespace kawaii {
using namespace std;

const mat4& Node::LocalTransform() {
    if (is_transform_dirty_) {
        float x = position_.x;
        float y = position_.y;
        float z = position_.z;

        float cx = 1;
        float sx = 0;
        float cy = 1;
        float sy = 0;
        if (rotation_x_ || rotation_y_) {
            float radians_x_ = -DegreesToRadians(rotation_x_);
            float radians_y_ = -DegreesToRadians(rotation_y_);
            cx = cosf(radians_x_);
            sx = sinf(radians_x_);
            cy = cosf(radians_y_);
            sy = sinf(radians_y_);
        }

        // optimization:
        // inline anchor point calculation if skew is not needed
        bool needs_skew_matrix = (skew_x_ || skew_y_);
        if (!needs_skew_matrix && !anchor_point_is_zero()) {
            x += cy * -anchor_point_in_points_.x * scale_x_ + -sx * -anchor_point_in_points_.y * scale_y_;
            y += sy * -anchor_point_in_points_.x * scale_x_ +  cx * -anchor_point_in_points_.y * scale_y_;
        }

        // Build Transform Matrix
        float a_a  = cy * scale_x_;
        float a_b  = sy * scale_x_;
        float a_c  = -sx * scale_y_;
        float a_d  = cx * scale_y_;
        local_transform_ = mat4(a_a, a_b, 0, 0,
                                a_c, a_d, 0, 0,
                                  0,   0, 1, 0,
                                  x,   y, z, 1);

        // XXX: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if (needs_skew_matrix) { 
            mat4 skew_matrix(1.0f, tanf(DegreesToRadians(skew_y_)), 0, 0,
                             tanf(DegreesToRadians(skew_x_)), 1.0f, 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1);
            local_transform_ = skew_matrix * local_transform_;

            // adjust anchor point
            if (!anchor_point_is_zero()) {
                local_transform_ = local_transform_.Translate(-anchor_point_in_points_.x, -anchor_point_in_points_.y, 0);
            }
        }

        is_transform_dirty_ = false;
    }

    return local_transform_;
}

mat4 Node::WorldTransform() {
    mat4 t = LocalTransform();
    for (shared_ptr<Node> p = parent(); p; p = p->parent()) {
        t *= p->LocalTransform();
    }
    return t;
}

vec3 Node::WorldPosition() {
    return WorldPositionAt(position_);
}

vec3 Node::WorldPositionAt(const vec3& position) {
    vec4 pos = vec4(position, 1) * WorldTransform();
    return vec3(pos.x, pos.y, pos.z);
}

vec3 Node::LocalPositionAt(const vec3& world_position) {
    return WorldPosition() - world_position;
}

void Node::AddChild(shared_ptr<Node> child) {
    assert(!child->parent());

    child->set_parent(shared_from_this());
    children_.push_back(child);
}

void Node::Visit() {
    if (!is_visible_) return;

    mat4& model_view = MatrixStack<GLModelView>::Instance().Push();

    model_view *= LocalTransform();

    if (!children_.empty()) {
        Render();

        // sort all children

        // Render children z_order < 0

        // Render();

        
        // Render children z_order > 0
        for (vector<shared_ptr<Node> >::iterator iter = children_.begin(); iter != children_.end(); iter++) {
            shared_ptr<Node> child = (*iter);
            child->Visit();
        }

    } else {
        Render();
    }

    MatrixStack<GLModelView>::Instance().Pop();
}

void Node::ScheduleUpdate() {
    scheduler().Attach(shared_from_this());
}

ProcessScheduler& Node::scheduler() const {
    return Application::Instance().director().scheduler();
}

EventDispatcher& Node::dispatcher() const {
    return Application::Instance().director().dispatcher();
}

}
