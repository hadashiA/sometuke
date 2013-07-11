#ifndef __skidarake__ios_component_factory__
#define __skidarake__ios_component_factory__

#include "skidarake/application.h"
#include "skidarake/ios/ios_assets_loader.h"

namespace skidarake {

class IOSComponentFactory : public ApplicationComponentFactory {
public:
    virtual AssetsLoader *CreateAssetsLoader() {
        return new IOSAssetsLoader;
    }
};

}

#endif /* defined(__skidarake__ios_component_factory__) */
