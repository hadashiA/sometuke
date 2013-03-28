#include "node.h"

#include "matrix_stack.h"
#include "application.h"
#include "logger.h"

#include <cassert>

namespace kawaii {
using namespace std;

const mat4& Node::LocalTransform() {
    if (is_transform_dirty_) {
        float x = local_position_.x;
        float y = local_position_.y;
        float z = local_position_.z;

        float c = 1;
        float s = 0;
        if (rotation_) {
            float radians = -DegreesToRadians(rotation_);
            c = cosf(radians);
            s = sinf(radians);
        }

        // optimization:
        // inline anchor point calculation if skew is not needed
        bool needs_skew_matrix = (skew_.x || skew_.y);
        if (!needs_skew_matrix && !anchor_point_is_zero()) {
            x += c * -anchor_point_.x * scale_.x + -s * -anchor_point_.y * scale_.y;
            y += s * -anchor_point_.x * scale_.x +  c * -anchor_point_.y * scale_.y;
        }

        // Build Transform Matrix
        float a_a  = c * scale_.x;
        float a_b  = s * scale_.x;
        float a_c  = -s * scale_.y;
        float a_d  = c * scale_.y;
        local_transform_ = mat4(a_a, a_b, 0, 0,
                                a_c, a_d, 0, 0,
                                  0,   0, 1, 0,
                                  x,   y, z, 1);

        // XXX: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if (needs_skew_matrix) { 
            mat4 skew_matrix(1.0f, tanf(DegreesToRadians(skew_.y)), 0, 0,
                             tanf(DegreesToRadians(skew_.x)), 1.0f, 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1);
            local_transform_ = skew_matrix * local_transform_;

            // adjust anchor point
            if (!anchor_point_is_zero()) {
                local_transform_ = local_transform_.Translate(-anchor_point_.x, -anchor_point_.y, 0);
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
    return WorldPositionAt(local_position_);
}

vec3 Node::WorldPositionAt(const vec3& local_position) {
    vec4 pos = vec4(local_position, 1) * WorldTransform();
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
    ProcessScheduler& scheduler = Application::Instance().director().scheduler();
    scheduler.Attach(shared_from_this());
}

}
