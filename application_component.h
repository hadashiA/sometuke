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

enum class LineBreakMode {
    WORD_WRAP,
    CHARACTER_WRAP,
    CLIP,
    HEAD_TRUNCATION,
    TAIL_TRUNCATION,
    MIDDLE_TRUNCATION,
};

struct FontShadow {
public:
    FontShadow()
        : enabled(false),
          blur(0),
          opacity(0) {
    }

    // true if shadow enabled
    bool enabled;
    // shadow x and y offset
    vec2 offset;
    // shadow blurrines
    float blur;
    // shadow opacity
    float opacity;
};

struct FontStroke {
public:
    // stroke is disabled by default
    FontStroke()
        : enabled(false),
          color(Color3B::BLACK),
          size(0) {
    }
    
    // true if stroke enabled
    bool enabled;
    // stroke color
    Color3B color;
    // stroke size
    float size;
    
};

// font attributes
struct FontDefinition {
public:
    FontDefinition()
        : size(0),
          h_alignment(TextHAlignment::CENTER),
          v_alignment(TextVAlignment::TOP),
          dimensions(0, 0),
          fill_color(Color3B::WHITE) {
    }
    
    string name;
    int size;
    TextHAlignment h_alignment;
    TextVAlignment v_alignment;
    LineBreakMode line_break_mode;
    vec2 dimensions;
    Color3B fill_color;
    FontShadow shadow;
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
    virtual bool InitWtihFile(const string& file) = 0;
    virtual bool InitWithText(const string& text, const string& font_name, int size,
                              int width = 0, int height = 0) = 0;
    virtual bool InitWithText(const string& text, const FontDefinition& font_def) = 0;

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

    int bits_per_component() {
        return bits_per_component_;
    }

protected:
    vector<unsigned char> bytes_;
    bool has_alpha_;
    bool pre_multi_;
    unsigned short width_;
    unsigned short height_;
    int bits_per_component_;

private:
    // noncopyable
    Image(const Image&);
    Image& operator=(const Image&);
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
    virtual vector<unsigned char> ReadBytes(const string& relative_path);
    virtual string ReadString(const string &relative_path);
};
    
class Texture2D;

class ImageLoader {
public:
    virtual ~ImageLoader() {}

    virtual shared_ptr<Image> Create() = 0;
    virtual shared_ptr<Texture2D> CreateTextureFromFile(const string& path) = 0;
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
