#ifndef __sometuke__application_component_factory__
#define __sometuke__application_component_factory__

#include "sometuke/vector.h"
#include "sometuke/color.h"

#include <vector>
#include <memory>
#include <string>

namespace sometuke {
using namespace std;

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

class Image {
public:
    enum class Format {
        JPG,
        PNG,
        TIFF,
        WEBP,
        ROWDATA,
        UNKNOWN,
    };

    Image()
        : width_(0),
          height_(0),
          bits_per_component_(0),
          has_alpha_(false),
          pre_multi_(false) {
    }

    virtual ~Image() {}

    unsigned char *data() {
        return bytes_.data();
    }

    const size_t size() const {
        return width_ * height_;
    }

    bool has_alpha() {
        return has_alpha_;
    }

    bool is_premultipled_alpha() {
        return pre_multi_;
    }

    unsigned short width() {
        return width_;
    }

    unsigned short height() {
        return height_;
    }

private:
    // noncopyable
    Image(const Image&);
    Image& operator=(const Image&);

    vector<unsigned char> bytes_;
    bool has_alpha_;
    bool pre_multi_;
    unsigned short width_;
    unsigned short height_;
    int bits_per_component_;
};

class Configuration {
public:
    virtual ~Configuration() {}
    virtual GLint MaxTextureSize() = 0;
    virtual GLint MaxTextureUnits() = 0;
};

class FileUtils {
public:
    virtual ~FileUtils() {}
    virtual const string FullPathFromRelativePath(const string& relative_path) = 0;
    virtual vector<char> ReadBytes(const string& relative_path);
    virtual string ReadString(const string &relative_path);
};
    
class Texture2D;

class ImageLoader {
public:
    virtual ~ImageLoader() {}

    virtual shared_ptr<Texture2D> CreateTextureFromFile(const string& path) = 0;
    virtual shared_ptr<Image> Create() = 0;
    // virtual shared_ptr<Image> CreateImageFromText(const string& text,
    //                                               const string& font_name,
    //                                               const float fond_size,
    //                                               const vec2& dimentions = vec2(0, 0),
    //                                               TextHAlignment h_alignment = TextHAlignment::LEFT,
    //                                               TextVAlignment v_alignment = TextVAlignment::TOP) = 0;
};

class ApplicationComponent {
public:
    virtual ~ApplicationComponent() {}
    virtual Configuration *CreateConfiguration() = 0;
    virtual FileUtils *CreateFileUtils() = 0;
    virtual ImageLoader *CreateImageLoader() = 0;
};

}

#endif /* defined(__sometuke__application_component_factory__) */
