#include "node.h"

namespace kawaii {
using namespace std;

void Node::AddChild(shared_ptr<Node> child) {
    children_.push_back(child);
}

void Node::OnEnter() {
}

void Node::Update(const float delta_time) {
}

void Node::Render() {
}

}
