#include "node.h"

#include "matrix_stack.h"

namespace kawaii {
using namespace std;

void Node::AddChild(shared_ptr<Node> child) {
    children_.push_back(child);
}

void Node::Visit() {
    if (!is_visible_) return;
}

}
