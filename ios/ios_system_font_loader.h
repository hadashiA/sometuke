#ifndef __sometuke__ios_ios_system_font_loader__
#define __sometuke__ios_ios_system_font_loader__

#include "sometuke/application_component.h"

namespace sometuke {

using namespace std;

class IOSSystemFontLoader : public SystemFontLoader {
public:
    shared_ptr<Texture2D> CreateTexture(const string& str,
                                        const string& font_name,
                                        float font_size);

    shared_ptr<Texture2D> CreateTexture(const string& str,
                                        const string& font_name,
                                        float font_size,
                                        const vec2& dimentions);
private:
    shared_ptr<Texture2D> CreateTextureWithString(NSString *);
    shared_ptr<Texture2D> CreateTextureWithAttributedString(NSAttributedString *);
};

}

#endif /* defined(__sometuke__ios_ios_system_font_loader__) */
