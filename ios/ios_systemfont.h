#ifndef __sometuke__ios_system_font_loader__
#define __sometuke__ios_system_font_loader__

#include <memory>

namespace sometuke {

using namespace std;

class Texture2D;

class IOSSystemFontLoader : public SystemFontLoader {
public:
    virtual ~IOSSystemFontLoader() {}
    virtual shared_ptr<Texture2D> TextureWithString();
};

}

#endif /* defined(__sometuke__ios_system_font_loader__) */
