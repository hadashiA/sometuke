#include "texture_2d.h"
#include "logger.h"

#include <assert.h>

namespace kawaii {

unsigned int Texture2D::BitsPerPixelForFormat(PixelFormat format) {
    unsigned int ret = 0;

    switch (format) {
    case kPixelFormat_RGBA8888:
        ret = 32;
        break;
    case kPixelFormat_RGB888:
        // It is 32 and not 24, since its internal representation uses 32 bits.
        ret = 32;
        break;
    case kPixelFormat_RGB565:
        ret = 16;
        break;
    case kPixelFormat_RGBA4444:
        ret = 16;
        break;
    case kPixelFormat_RGB5A1:
        ret = 16;
        break;
    case kPixelFormat_AI88:
        ret = 16;
        break;
    case kPixelFormat_A8:
        ret = 8;
        break;
    case kPixelFormat_I8:
        ret = 8;
        break;
    case kPixelFormat_PVRTC4:
        ret = 4;
        break;
    case kPixelFormat_PVRTC2:
        ret = 2;
        break;
    default:
        ret = -1;
        IIERROR("bitsPerPixelForFormat: %ld, cannot give useful result", (long)format);
        assert(false);
        break;
    }

    return ret;
}

Texture2D::Texture2D()
    : id_(0),
      pixel_size_(0, 0),
      content_size_(0, 0),
      format_(kPixelFormat_Default) {
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture2D::LoadData(const void* data, PixelFormat pixel_format,
                         ivec2 pixel_size, vec2 content_size) {
    int pixel_width  = pixel_size.x;
    int pixel_height = pixel_size.y;
    
    if (pixel_format == kPixelFormat_RGBA8888 ||
        (NextPowerOf2(pixel_width) == pixel_width && NextPowerOf2(pixel_height) == pixel_height)) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    } else {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    switch(pixel_format) {
    case kPixelFormat_RGBA8888:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;

    case kPixelFormat_RGBA4444:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
        break;

    case kPixelFormat_RGB5A1:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);
        break;

    case kPixelFormat_RGB565:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
        break;

    case kPixelFormat_RGB888:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;

    case kPixelFormat_AI88:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
        break;

    case kPixelFormat_A8:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
        break;
    default:
        IIERROR("invalid pixel format %d", pixel_format);
    }

    content_size_ = content_size;
    pixel_size_   = pixel_size;
    format_       = pixel_format;
    max_s_ = content_size_.x / pixel_size_.x;
    max_t_ = content_size.y  / pixel_size_.y;

    has_premultipled_alpha_ = false;

    ShaderCache shaders = *ShaderCache::Shared();
    shader_program_ = shaders[kShader_PositionTexture];
}

void Texture2D::_LoadData(const void* data, PixelFormat pixel_format,
                         ivec2 pixel_size, vec2 content_size) {
    int pixel_width  = pixel_size.x;
    int pixel_height = pixel_size.y;
    
    if (pixel_format == kPixelFormat_RGBA8888 ||
        (NextPowerOf2(pixel_width) == pixel_width && NextPowerOf2(pixel_height) == pixel_height)) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    } else {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    switch(pixel_format) {
    case kPixelFormat_RGBA8888:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;

    case kPixelFormat_RGBA4444:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
        break;

    case kPixelFormat_RGB5A1:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);
        break;

    case kPixelFormat_RGB565:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
        break;

    case kPixelFormat_RGB888:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;

    case kPixelFormat_AI88:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
        break;

    case kPixelFormat_A8:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
                     (GLsizei)pixel_width, (GLsizei)pixel_height,
                     0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
        break;
    default:
        IIERROR("invalid pixel format %d", pixel_format);
    }

    content_size_ = content_size;
    pixel_size_   = pixel_size;
    format_       = pixel_format;
    max_s_ = content_size_.x / pixel_size_.x;
    max_t_ = content_size.y  / pixel_size_.y;

    has_premultipled_alpha_ = false;

    ShaderCache shaders = *ShaderCache::Shared();
    shader_program_ = shaders[kShader_PositionTexture];
}

}
