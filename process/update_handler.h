#ifndef __kawaii__process_delegate__
#define __kawaii__process_delegate__

#include "kawaii/process/process.h"

#include <memory>

namespace kawaii {
using namespace std;

class UpdateInterface {
public:
    virtual ~UpdateInterface() {}
    virtual void Update(const ii_time delta_time) = 0;
    virtual const bool paused() = 0;
};

class UpdateHandler : public Process {
public:
    static const HashedString TYPE;

    UpdateHandler(weak_ptr<UpdateInterface> update_entry)
        : entry_(update_entry) {
    }

    virtual const HashedString& type() const {
        return UpdateHandler::TYPE;
    }

    virtual size_t target_id() {
        if (shared_ptr<UpdateInterface> entry = entry_.lock()) {
            return reinterpret_cast<size_t>(entry.get());
        } else {
            return Process::target_id();
        }
    }

    virtual bool Update(const ii_time delta) {
        if (shared_ptr<UpdateInterface> entry = entry_.lock()) {
            if (!entry->paused()) {
                entry->Update();
            }
        } else {
            return false;
        }
    }

private:
    weak_ptr<UpdateInterface> entry_;
}

}

#endif /* defined(__kawaii__process_delegate__) */
