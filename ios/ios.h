#ifndef __sometuke__ios_component_factory__
#define __sometuke__ios_component_factory__

#include "sometuke/application_component.h"
#include "sometuke/ios/ios_assets_loader.h"
#include "sometuke/ios/ios_system_font_renderer.h"

namespace sometuke {

class IOSComponent : public ApplicationComponent {
public:
    virtual ~IOSComponent() {}

    virtual AssetsLoader *CreateAssetsLoader() {
        return new IOSAssetsLoader;
    }

    virtual SystemFontRenderer *CreateSystemFontRenderer() {
        return new IOSSystemFontRenderer;
    }
};

}

#endif /* defined(__sometuke__ios_component_factory__) */
