#include "sometuke/node/node.h"

#include "sometuke/matrix_stack.h"
#include "sometuke/logger.h"

#include <cassert>

namespace sometuke {
using namespace std;

const mat4& Node::LocalTransform() {
    if (is_transform_dirty_) {
        float x = position_.x;
        float y = position_.y;
        float z = position_.z;

        if (ignore_anchor_point_for_position_) {
            x += anchor_point_in_points_.x;
            y += anchor_point_in_points_.y;
        }

        float c = 1;
        float s = 0;
        if (rotation_) {
            float radians = -DegreesToRadians(rotation_);
            c = cosf(radians);
            s = sinf(radians);
        }

        // optimization:
        // inline anchor point calculation if skew is not needed
        bool needs_skew_matrix = (skew_x_ || skew_y_);
        if (!needs_skew_matrix && !anchor_point_is_zero()) {
            x += c * -anchor_point_in_points_.x * scale_x_ + -s * -anchor_point_in_points_.y * scale_y_;
            y += s * -anchor_point_in_points_.x * scale_x_ +  c * -anchor_point_in_points_.y * scale_y_;
        }

        // Build Transform Matrix
        float a_a  = c * scale_x_;
        float a_b  = s * scale_x_;
        float a_c  = -s * scale_y_;
        float a_d  = c * scale_y_;
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

void Node::AddChild(const shared_ptr<Node>& child) {
    assert(!child->parent());

    child->set_parent(static_pointer_cast<Node>(shared_from_this()));
    children_.push_back(child);

    child->Enter();
}

void Node::RemoveChild(const shared_ptr<Node>& child) {
    if (child) {
        for (vector<shared_ptr<Node> >::iterator i = children_.begin(); i != children_.end();) {
            if (*i == child) {
                child->Exit();
                i = children_.erase(i);
                break;
            } else {
                ++i;
            }
        }
    }
}

void Node::Visit() {
    if (!is_visible_) return;

    mat4& model_view = MatrixStack<GLModelView>::Instance().Push();

    model_view = LocalTransform() * model_view;

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

}
