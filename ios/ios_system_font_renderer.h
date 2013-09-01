#ifndef __sometuke__ios_ios_system_font_loader__
#define __sometuke__ios_ios_system_font_loader__

#include "sometuke/application_component.h"

namespace sometuke {

using namespace std;

class IOSSystemFontRenderer : public SystemFontRenderer {
public:
    shared_ptr<Texture2D> CreateTexture(const string& text,
                                        const FontDefinition& font_def);
    
    shared_ptr<Texture2D> CreateTexture(const string& text,
                                        const string& font_name,
                                        const float fond_size,
                                        const vec2& dimentions = vec2(0, 0),
                                        TextHAlignment h_alignment = TextHAlignment::LEFT,
                                        TextVAlignment v_alignment = TextVAlignment::TOP);
private:
};

}

#endif /* defined(__sometuke__ios_ios_system_font_loader__) */
