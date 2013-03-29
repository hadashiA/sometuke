#ifndef __kawaii__process_node_process_relation__
#define __kawaii__process_node_process_relation__

#include "sequence.h"
#include "memory_pool.h"
#include "vector.h"

#include <memory>

namespace kawaii {

class Node;
class Sequence;

class NodeProcessRelation {
public:
    NodeProcessRelation(weak_ptr<Node> target)
        : target_(target),
          sequence_(new Sequence) {
    }

    virtual ~NodeProcessRelation() {}

    void Run();
    void Repeat(int n = -1);

    NodeProcessRelation MoveTo(const ii_time duration, const vec3& to);
    NodeProcessRelation MoveBy(const ii_time duration, const vec3& delta);

private:
    weak_ptr<Node> target_;
    shared_ptr<Sequence> sequence_;
};

}

#endif /* defined(__kawaii__process_node_process_relation__) */
