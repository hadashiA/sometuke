#include "node.h"

#include "matrix_stack.h"

#include "logger.h"

namespace kawaii {
using namespace std;

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
