#include "sometuke/ios/ios_system_font_renderer.h"

#include "sometuke/ios/devices.h"
#include "sometuke/texture_2d.h"

namespace sometuke {

shared_ptr<Texture2D> IOSSystemFontRenderer::CreateTexture(const string& text,
                                                           const FontDefinition& font_def) {
    return make_shared<Texture2D>();
}
    
shared_ptr<Texture2D> IOSSystemFontRenderer::CreateTexture(const string& text,
                                                           const string& font_name,
                                                           const float fond_size,
                                                           const vec2& dimensions,
                                                           TextHAlignment h_alignment,
                                                           TextVAlignment v_alignment) {
    FontDefinition font_def;
    
    font_def.shadow.shadow_enabled = false;
    font_def.stroke.stroke_enabled = false;

    font_def.font_name       = font_name;
    font_def.dimensions      = dimensions;
    font_def.h_alignment     = h_alignment;
    font_def.v_alignment     = v_alignment;
    font_def.font_fill_color = Color3B::WHITE;

    return CreateTexture(text, font_def);
}


}
