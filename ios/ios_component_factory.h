#ifndef __kawaii__ios_component_factory__
#define __kawaii__ios_component_factory__

#include "application.h"
#include "ios_assets_loader.h"

namespace kawaii {

class IOSComponentFactory : public ApplicationComponentFactory {
public:
    virtual AssetsLoader *CreateAssetsLoader() {
        return new IOSAssetsLoader;
    }
};

}

#endif /* defined(__kawaii__ios_component_factory__) */
