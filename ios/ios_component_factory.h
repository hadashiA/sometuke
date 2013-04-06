#ifndef __kawaii__ios_component_factory__
#define __kawaii__ios_component_factory__

#include "application.h"
#include "ios_assets_loader.h"
#include "ios_control_dispatcher.h"

namespace kawaii {

class IOSComponentFactory : public ApplicationComponentFactory {
public:
    virtual AssetsLoader *CreateAssetsLoader() {
        return new IOSAssetsLoader;
    }

    virtual ControlDispatcher *CreateControlDispatcher() {
        return new IOSControlDispatcher;
    }
};

}

#endif /* defined(__kawaii__ios_component_factory__) */
