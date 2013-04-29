#ifndef __kawaii__process_node_process__
#define __kawaii__process_node_process__

#include "kawaii/process/process.h"
#include "kawaii/node/node.h"

#include <memory>

namespace kawaii {

class NodeProcess : public Process {
public:
    NodeProcess(weak_ptr<Node> target)
        : target_(target) {
    }

    virtual ~NodeProcess() {}

    virtual size_t target_id() {
        if (shared_ptr<Node> node = target_.lock()) {
            return reinterpret_cast<size_t>(node.get());
        } else {
            return Process::target_id();
        }
    }

protected:
    weak_ptr<Node> target_;
};

}

#endif /* defined(__kawaii__process_node_process__) */
