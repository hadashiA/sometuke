#include "node.h"

#include "matrix_stack.h"

#include "logger.h"

namespace kawaii {
using namespace std;

void Node::set_local_position(const vec3& position) {
    local_position_ = position;
    is_transform_dirty_ = is_inverse_dirty_ = true;
}

void Node::add_position(const vec3& diff) {
    local_position_ += diff;
    is_transform_dirty_ = is_inverse_dirty_ = true;
}

void Node::set_scale_x(const float scale_x) {
    scale_.x = scale_x;
    is_transform_dirty_ = is_inverse_dirty_ = true;
}

void Node::set_scale_y(const float scale_y) {
    scale_.y = scale_y;
    is_transform_dirty_ = is_inverse_dirty_ = true;
}

void Node::set_scale(const float scale) {
    scale_.x = scale;
    scale_.y = scale;
    is_transform_dirty_ = is_inverse_dirty_ = true;
}

void Node::set_skew_x(const float skew_x) {
    skew_.x = skew_x;
    is_transform_dirty_ = is_inverse_dirty_ = true;
}

void Node::set_skew_y(const float skew_y) {
    skew_.y = skew_y;
    is_transform_dirty_ = is_inverse_dirty_ = true;
}

void Node::set_z_order(int z_order) {
    z_order = z_order;
    // parent_->ReorderChild(this, z_order)
}

void Node::set_anchor_point(const vec2& value) {
    if (anchor_point_ != value) {
        anchor_point_ = value;
        anchor_point_ = vec2(content_size_.x * anchor_point_.x,
                             content_size_.y * anchor_point_.y);
        is_transform_dirty_ = is_inverse_dirty_ = true;
    }
}

void Node::set_content_size(const vec2& value) {
    if (content_size_ != value) {
        content_size_ = value;
        anchor_point_ = vec2(content_size_.x * anchor_point_.x,
                             content_size_.y * anchor_point_.y);
        is_transform_dirty_ = is_inverse_dirty_ = true;
    }
}

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
        if (!needs_skew_matrix && !AnchorPointIsZero()) {
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
                          tx, ty, 0, 1);

        // XXX: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if (needs_skew_matrix) { 
            mat4 skew_matrix(1.0f, tanf(DegreesToRadians(skew_.y)), 0, 0,
                             tanf(DegreesToRadians(skew_.x)), 1.0f, 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1);
            transform_ = skew_matrix * transform_;

            // adjust anchor point
            if (!AnchorPointIsZero()) {
                transform_ = transform_.Translate(-anchor_point_.x, -anchor_point_.y, 0);
            }
        }

        is_transform_dirty_ = false;
    }

    return transform_;
}

const mat4& Node::ParentToNodeTransform() {
    if (is_inverse_dirty_) {
        // is_inverse_dirty_ = transform_.Inverse();
        is_inverse_dirty_ = false;
    }
    return inverse_;
}

void Node::AddChild(shared_ptr<Node> child) {
    children_.push_back(child);
}

void Node::Visit() {
    if (!is_visible_) return;

    MatrixStack::GLModelView()->Push();

    if (!children_.empty()) {
        for (vector<shared_ptr<Node> >::iterator iter = children_.begin(); iter != children_.end(); iter++) {
            shared_ptr<Node> child = (*iter);
            child->Visit();
        }
    }
    Render();

    MatrixStack::GLModelView()->Pop();
}

}
