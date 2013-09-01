#ifndef __sometuke__application_component_factory__
#define __sometuke__application_component_factory__

#include "sometuke/vector.h"

#include "color.h"

#include <vector>
#include <memory>
#include <string>

namespace sometuke {
using namespace std;

class Texture2D;

// ???
// class Image {
// public:
// };

enum class TextHAlignment {
    LEFT,
    CENTER,
    RIGHT,
};

enum class TextVAlignment {
    TOP,
    CENTER,
    BOTTOM,
};

struct FontShadow {
public:
    FontShadow()
        : shadow_enabled(false),
          shadow_blur(0),
          shadow_opacity(0) {
    }

    // true if shadow enabled
    bool shadow_enabled;
    // shadow x and y offset
    vec2 shadow_offset;
    // shadow blurrines
    float shadow_blur;
    // shadow opacity
    float shadow_opacity;
};

struct FontStroke {
public:
    // stroke is disabled by default
    FontStroke()
        : stroke_enabled(false),
          stroke_color(Color3B::BLACK),
          stroke_size(0) {
    }
    
    // true if stroke enabled
    bool stroke_enabled;
    // stroke color
    Color3B stroke_color;
    // stroke size
    float stroke_size;
    
};

// font attributes
struct FontDefinition {
public:
    FontDefinition()
        : font_size(0),
          h_alignment(TextHAlignment::CENTER),
          v_alignment(TextVAlignment::TOP),
          dimensions(0, 0),
          font_fill_color(Color3B::WHITE) {
    }
    
    string font_name;
    int font_size;
    TextHAlignment h_alignment;
    TextVAlignment v_alignment;

    // renering box
    vec2 dimensions;
    // font color
    Color3B font_fill_color;
    // font shadow
    FontShadow shadow;
    // font stroke
    FontStroke stroke;
    
};

class AssetsLoader {
public:
    virtual ~AssetsLoader() {}
    virtual vector<char> ReadBytes(const string& relative_path) = 0;
    virtual shared_ptr<Texture2D> ReadTexture(const string &relative_path) = 0;
    virtual string ReadString(const string &relative_path) = 0;
    virtual const string FullPathFromRelativePath(const string& relative_path) = 0;
};

class SystemFontRenderer {
public:
    virtual ~SystemFontRenderer() {}

    virtual shared_ptr<Texture2D> CreateTexture(const string& text,
                                                const FontDefinition& font_def) = 0;
    
    virtual shared_ptr<Texture2D> CreateTexture(const string& text,
                                                const string& font_name,
                                                const float fond_size,
                                                const vec2& dimentions = vec2(0, 0),
                                                TextHAlignment h_alignment = TextHAlignment::LEFT,
                                                TextVAlignment v_alignment = TextVAlignment::TOP) = 0;
};

class ApplicationComponent {
public:
    virtual ~ApplicationComponent() {}
    virtual AssetsLoader *CreateAssetsLoader() = 0;
    virtual SystemFontRenderer *CreateSystemFontRenderer() = 0;
};

}

#endif /* defined(__sometuke__application_component_factory__) */
