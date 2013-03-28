#ifndef __kawaii__process_target__
#define __kawaii__process_target__

#include "application.h"
#include "memory_pool.h"

#include <memory>

namespace kawaii {

class Node;
class Sequence;

class NodeProcessRelation : public GeneralPoolable {
public:
    NodeProcessRelation(weak_ptr<Node> target)
        : target_(target) {}

    virtual ~NodeProcessRelation() {
        Application::Instance().director().scheduler().Attach(sequence_);
    }

private:
    weak_ptr<Node> target_;
    shared_ptr<Process> sequence_;
};

NodeProcessRelation Target(weak_ptr<Node> target) {
    return NodeProcessRelation(target);
}

}

#endif /* defined(__kawaii__process_target__) */





