#ifndef __sometuke__application_component_factory__
#define __sometuke__application_component_factory__

#include "sometuke/vector.h"

#include <vector>
#include <memory>
#include <string>

namespace sometuke {
using namespace std;

class Texture2D;

class AssetsLoader {
public:
    virtual ~AssetsLoader() {}
    virtual vector<char> ReadBytes(const string& relative_path) = 0;
    virtual shared_ptr<Texture2D> ReadTexture(const string &relative_path) = 0;
    virtual string ReadString(const string &relative_path) = 0;
    virtual const string FullPathFromRelativePath(const string& relative_path) = 0;
};

class SystemFontLoader {
public:    
    typedef enum {
        kTextAlignmentLeft,
        kTextAlignmentCenter,
        kTextAlignmentRight,
    } TextAlignment;

    typedef enum {
        kLineBreakModeWordWrap,
        kLineBreakModeCharacterWrap,
        kLineBreakModeClip,
        kLineBreakModeHeadTruncation,
        kLineBreakModeTailTruncation,
        kLineBreakModeMiddleTruncation,
    } LineBreakMode;

    virtual ~SystemFontLoader() {}

    virtual shared_ptr<Texture2D> CreateTexture(const string& str,
                                                const string& font_name,
                                                float font_size) = 0;

    virtual shared_ptr<Texture2D> CreateTexture(
        const string& str,
        const string& font_name,
        const float fond_size,
        const vec2& dimentions,
        TextAlignment h_alignment,
        TextAlignment v_alignment,
        LineBreakMode line_break_mode = kLineBreakModeWordWrap) = 0;
};

class ApplicationComponent {
public:
    virtual ~ApplicationComponent() {}
    virtual AssetsLoader *CreateAssetsLoader() = 0;
    virtual SystemFontLoader *CreateSystemFontLoader() = 0;
};

}

#endif /* defined(__sometuke__application_component_factory__) */
