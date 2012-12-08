#ifndef __kawaii__texture_2d__
#define __kawaii__texture_2d__

#include <memory>

#include "vector.h"
#include "shader_cache.h"

namespace kawaii {

typedef enum {
    //! 32-bit texture: RGBA8888
    kTexture2DPixelFormat_RGBA8888,
    //! 32-bit texture without Alpha channel. Don't use it.
    kTexture2DPixelFormat_RGB888,
    //! 16-bit texture without Alpha channel
    kTexture2DPixelFormat_RGB565,
    //! 8-bit textures used as masks
    kTexture2DPixelFormat_A8,
    //! 8-bit intensity texture
    kTexture2DPixelFormat_I8,
    //! 16-bit textures used as masks
    kTexture2DPixelFormat_AI88,
    //! 16-bit textures: RGBA4444
    kTexture2DPixelFormat_RGBA4444,
    //! 16-bit textures: RGB5A1
    kTexture2DPixelFormat_RGB5A1,
    //! 4-bit PVRTC-compressed texture: PVRTC4
    kTexture2DPixelFormat_PVRTC4,
    //! 2-bit PVRTC-compressed texture: PVRTC2
    kTexture2DPixelFormat_PVRTC2,
    
    //! Default texture format: RGBA8888
    kTexture2DPixelFormat_Default = kTexture2DPixelFormat_RGBA8888,

} Texture2DPixelFormat;

class Texture2D {
public:
    Texture2D() : id_(0), pixelSize_(0, 0), contentSize_(0, 0),
                  format_(kTexture2DPixelFormat_Default) {}

    void LoadData(const void* data, Texture2DPixelFormat pixelFormat,
                  ivec2 pixelSize_, vec2 contentSize_);
private:
    GLuint id_;
    ivec2 pixelSize_;
    vec2 contentSize_;
    Texture2DPixelFormat format_;
    GLfloat max_s_;
    GLfloat max_t_;
    bool has_premultipled_alpha_;
    bool has_mipmaps_;
    
    shared_ptr<GLProgram> shaderProgram_;
};

}

#endif /* defined(__kawaii__texture_2d__) */
