#ifndef __sometuke__ios_ios_system_font_loader__
#define __sometuke__ios_ios_system_font_loader__

#include "sometuke/application_component.h"

namespace sometuke {

using namespace std;

class IOSSystemFontLoader : public SystemFontLoader {
public:
    shared_ptr<Texture2D> CreateTexture(const string& str,
                                        const string& font_name,
                                        float font_size) {
        return shared_ptr<Texture2D>();
    }

    shared_ptr<Texture2D> CreateTexture(
        const string& str,
        const string& font_name,
        const float fond_size,
        const vec2& dimentions,
        TextAlignment h_alignment,
        TextAlignment v_alignment,
        LineBreakMode line_break_mode = kLineBreakModeWordWrap) {
        return shared_ptr<Texture2D>();
    }
};

}

#endif /* defined(__sometuke__ios_ios_system_font_loader__) */
