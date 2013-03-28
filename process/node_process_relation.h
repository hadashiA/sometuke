#ifndef __kawaii__process_node_process_relation__
#define __kawaii__process_node_process_relation__

#include "sequence.h"
#include "application.h"
#include "memory_pool.h"

#include <memory>

namespace kawaii {

class Node;
class Sequence;

class NodeProcessRelation : public GeneralPoolable {
public:
    NodeProcessRelation(weak_ptr<Node> target)
        : target_(target),
          sequence_(new Sequence) {
    }

    virtual ~NodeProcessRelation() {}

    void Run() {
        Application::Instance().director().scheduler().Attach(sequence_);
    }

    NodeProcessRelation MoveTo(const ii_time duration, const vec3& to);
    NodeProcessRelation MoveBy(const ii_time duration, const vec3& delta);

private:
    weak_ptr<Node> target_;
    shared_ptr<Process> sequence_;
};

NodeProcessRelation Target(weak_ptr<Node> target);

}

#endif /* defined(__kawaii__process_node_process_relation__) */
