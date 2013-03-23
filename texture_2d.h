#ifndef __kawaii__texture_2d__
#define __kawaii__texture_2d__

#include <memory>

#include "vector.h"
#include "shader_cache.h"
#include "application.h"

namespace kawaii {

static inline unsigned long NextPowerOf2(unsigned long x) {
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >>16);
    return x + 1;
}

class Texture2D {
public:
    typedef enum {
        //! 32-bit texture: RGBA8888
        kPixelFormat_RGBA8888,
        //! 32-bit texture without Alpha channel. Don't use it.
        kPixelFormat_RGB888,
        //! 16-bit texture without Alpha channel
        kPixelFormat_RGB565,
        //! 8-bit textures used as masks
        kPixelFormat_A8,
        //! 8-bit intensity texture
        kPixelFormat_I8,
        //! 16-bit textures used as masks
        kPixelFormat_AI88,
        //! 16-bit textures: RGBA4444
        kPixelFormat_RGBA4444,
        //! 16-bit textures: RGB5A1
        kPixelFormat_RGB5A1,
        //! 4-bit PVRTC-compressed texture: PVRTC4
        kPixelFormat_PVRTC4,
        //! 2-bit PVRTC-compressed texture: PVRTC2
        kPixelFormat_PVRTC2,
    
        //! Default texture format: RGBA8888
        kPixelFormat_Default = kPixelFormat_RGBA8888,

    } PixelFormat;

    static unsigned int BitsPerPixelForFormat(PixelFormat format);
    
    
    Texture2D()
        : id_(0),
          pixel_size_(0, 0),
          content_size_(0, 0),
          format_(kPixelFormat_Default) {
        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);
    }

    const GLuint id() const {
        return id_;
    }

    const shared_ptr<GLProgram> shader_program() const {
        return shader_program_;
    }

    const bool has_premultipled_alpha() const {
        return has_premultipled_alpha_;
    }

    void set_has_premultipled_alpha(bool value) {
        has_premultipled_alpha_ = value;
    }

    const vec2 pixel_size() const {
        return pixel_size_;
    }

    const vec2 content_size() const {
        return content_size_ / Application::Instance().content_scale_factor();
    }

    bool LoadData(const void* data, PixelFormat pixel_format,
                  ivec2 pixelSize_, vec2 contentSize_);

private:
    GLuint id_;
    ivec2 pixel_size_;
    vec2 content_size_;
    PixelFormat format_;
    GLfloat max_s_;
    GLfloat max_t_;
    bool has_premultipled_alpha_;
    bool has_mipmaps_;
    
    shared_ptr<GLProgram> shader_program_;
};

}

#endif /* defined(__kawaii__texture_2d__) */
