#include "node.h"

#include "matrix_stack.h"

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
        float xx = c * scale_.x;
        float xy = s * scale_.x;
        float yx = -s * scale_.y;
        float yy = c * scale_.y;
        float tx = x;
        float ty = y;
        local_transform_ = mat4(xx, xy, 0, 0,
                                yx, yy, 0, 0,
                                 0,  0, 1, 0,
                                tx, ty, z, 1);

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

const mat4& Node::LocalInverseTransform() {
    if (is_inverse_dirty_) {
        // inverse_ = transform_.Inverse();
        is_inverse_dirty_ = false;
    }
    return local_inverse_;
}

mat4 Node::WorldTransform() {
    mat4 t = LocalTransform();
    for (Node *p = parent(); p != NULL; p = p->parent()) {
        t *= p->LocalTransform();
    }
    return t;
}

mat4 Node::WorldInverseTransform() {
    return mat4();
}

vec3 Node::WorldPosition() {
    vec4 pos = WorldTransform() * vec4(local_position_, 1);
    return vec3(pos.x, pos.y, pos.z);
}

void Node::AddChild(shared_ptr<Node> child) {
    assert(child->parent() == NULL);

    child->set_parent(this);

    children_.push_back(child);
}

void Node::Visit() {
    if (!is_visible_) return;

    mat4& model_view = MatrixStack::GLModelView()->Push();

    model_view = model_view * LocalTransform();

    if (!children_.empty()) {
        Render();

        // sort all children

        // Render children z_order < 0

        Render();

        
        // Render children z_order > 0
        for (vector<shared_ptr<Node> >::iterator iter = children_.begin(); iter != children_.end(); iter++) {
            shared_ptr<Node> child = (*iter);
            child->Visit();
        }

    } else {
        Render();
    }

    MatrixStack::GLModelView()->Pop();
}

}
