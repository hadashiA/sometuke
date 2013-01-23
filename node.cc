#include "node.h"

#include "matrix_stack.h"

#include "logger.h"

#include <cassert>

namespace kawaii {
using namespace std;

const mat4& Node::NodeToParentTransform() {
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
        float a = c * scale_.x;
        float b = s * scale_.x;
        float c_ = -s * scale_.y;
        float d = c * scale_.y;
        float tx = x;
        float ty = y;
        transform_ = mat4(a, b, 0, 0,
                          c_, d, 0, 0,
                          0, 0, 1, 0,
                          tx, ty, z, 1);

        // XXX: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if (needs_skew_matrix) { 
            mat4 skew_matrix(1.0f, tanf(DegreesToRadians(skew_.y)), 0, 0,
                             tanf(DegreesToRadians(skew_.x)), 1.0f, 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1);
            transform_ = skew_matrix * transform_;

            // adjust anchor point
            if (!anchor_point_is_zero()) {
                transform_ = transform_.Translate(-anchor_point_.x, -anchor_point_.y, 0);
            }
        }

        is_transform_dirty_ = false;
    }

    return transform_;
}

const mat4& Node::ParentToNodeTransform() {
    if (is_inverse_dirty_) {
        // inverse_ = transform_.Inverse();
        is_inverse_dirty_ = false;
    }
    return inverse_;
}

const mat4 NodeToWorldTransform() {
    mat4 t = NodeToWorldTransform();
    return t;
}

const mat4 WorldToNodeTransform() {
    return mat4();
}

void Node::AddChild(shared_ptr<Node> child) {
    assert(child->parent() == NULL);

    children_.push_back(child);
}

void Node::Visit() {
    if (!is_visible_) return;

    mat4& model_view = MatrixStack::GLModelView()->Push();

    // model_view *= NodeToParentTransform();
    model_view = model_view * NodeToParentTransform();

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
