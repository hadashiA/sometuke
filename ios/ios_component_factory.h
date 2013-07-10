#ifndef __skidarak__ios_component_factory__
#define __skidarak__ios_component_factory__

#include "skidarak/application.h"
#include "skidarak/ios/ios_assets_loader.h"

namespace skidarak {

class IOSComponentFactory : public ApplicationComponentFactory {
public:
    virtual AssetsLoader *CreateAssetsLoader() {
        return new IOSAssetsLoader;
    }
};

}

#endif /* defined(__skidarak__ios_component_factory__) */
