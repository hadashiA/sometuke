#include "node.h"

#include "matrix_stack.h"

#include "logger.h"

namespace kawaii {
using namespace std;

void Node::set_position(const vec3& position) {
    position_ = position;
    is_transform_dirty_ = true;
}

void Node::add_position(const vec3& diff) {
    position_ += diff;
    is_transform_dirty_ = true;
}

void Node::set_scale_x(const float scale_x) {
    scale_.x = scale_x;
    is_transform_dirty_ = true;
}

void Node::set_scale_y(const float scale_y) {
    scale_.y = scale_y;
    is_transform_dirty_ = true;
}

void Node::set_scale(const float scale) {
    scale_.x = scale;
    scale_.y = scale;
    is_transform_dirty_ = true;
}

void Node::set_z_order(int z_order) {
    z_order = z_order;
    // parent_->ReorderChild(this, z_order)
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
