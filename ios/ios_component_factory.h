#ifndef __sometuke__ios_component_factory__
#define __sometuke__ios_component_factory__

#include "sometuke/application.h"
#include "sometuke/ios/ios_assets_loader.h"

namespace sometuke {

class IOSComponentFactory : public ApplicationComponentFactory {
public:
    virtual AssetsLoader *CreateAssetsLoader() {
        return new IOSAssetsLoader;
    }
};

}

#endif /* defined(__sometuke__ios_component_factory__) */
