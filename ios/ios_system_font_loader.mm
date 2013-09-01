#include "sometuke/ios/ios_system_font_loader.h"

#include "sometuke/ios/devices.h"
#include "sometuke/texture_2d.h"

namespace sometuke {

shared_ptr<Texture2D> IOSSystemFontLoader::CreateTexture(const string& str,
                                                         const string& font_name,
                                                         float font_size) {
    // if (RunningDevise() < kiOSVersion_6_0_0) {
    if (true) {
        
    } else {
        // not implemented
    }
    return make_shared<Texture2D>();
}


shared_ptr<Texture2D> IOSSystemFontLoader::CreateTexture(const string& str,
                                                         const string& font_name,
                                                         float font_size,
                                                         const vec2& dimentions) {
    return make_shared<Texture2D>();
}

// private

shared_ptr<Texture2D> IOSSystemFontLoader::CreateTextureWithString(NSString *) {
    return make_shared<Texture2D>();
}

shared_ptr<Texture2D> IOSSystemFontLoader::CreateTextureWithAttributedString(NSAttributedString *) {
    // not implemented
    return make_shared<Texture2D>();
}


}
