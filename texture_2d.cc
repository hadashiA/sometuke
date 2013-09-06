#include "sometuke/texture_2d.h"
#include "sometuke/logger.h"

#include <assert.h>

namespace sometuke {

bool Texture2D::InitWithData(const void* data, PixelFormat pixel_format,
                             const ivec2& pixel_size, const vec2& content_size) {
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

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
        S2ERROR("invalid pixel format %d", pixel_format);
        return false;
    }

    content_size_ = content_size;
    pixel_size_   = pixel_size;
    format_       = pixel_format;
    max_s_ = content_size_.x / pixel_size_.x;
    max_t_ = content_size_.y  / pixel_size_.y;

    has_premultiplied_alpha_ = false;
    has_mipmaps_ = false;

    shader_program_ = ShaderCache::Instance().get(kShader_PositionTexture);
    return true;
}

bool Texture2D::InitWithImage(const shared_ptr<Image>& image) {
    if (!image) {
        S2ERROR("Cannt create texture. Image is null");
        return false;
    }

    unsigned int image_width  = image->width();
    unsigned int image_height = image->height();
    Configuration& conf = Director::Instance().configuration();
    unsigned int max_texture_size = conf.MaxTextureSize();
    if (image_width > max_texture_size || image_height > max_texture_size) {
        S2ERROR("image (%u x %u) is bigger than the supported %u x %u",
                image_width, image_height, max_texture_size, max_texture_size);
        return false;
    }

    unsigned int   *in_pixel_32  = nullptr;
    unsigned char  *in_pixel_8   = nullptr;
    unsigned short *out_pixel_16 = nullptr;

    unsigned char *temp_data = image->data();
    bool has_alpha = image->has_alpha();

    vec2 image_size = vec2(image->width(), image->height());
    PixelFormat pixel_format;
    size_t bpp = image->bits_per_component();

    // compute pixel format
    if (has_alpha) {
    	pixel_format = kPixelFormat_Default;

    } else {
        if (bpp >= 8) {
            pixel_format = kPixelFormat_RGB888;
        } else {
            pixel_format = kPixelFormat_RGB565;
        }
    }
    
    // Repack the pixel data into the right format
    unsigned int length = image_size.x * image_size.y;

    if (pixel_format == kPixelFormat_RGB565) {
        if (has_alpha) {
            // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
            temp_data    = new unsigned char[image_width * image_height * 2];
            out_pixel_16 = reinterpret_cast<unsigned short *>(temp_data);
            in_pixel_32  = reinterpret_cast<unsigned int *>(image->data());
            
            for(unsigned int i = 0; i < length; ++i, ++in_pixel_32) {
                *out_pixel_16++ = 
                    ((((*in_pixel_32 >>  0) & 0xFF) >> 3) << 11) |  // R
                    ((((*in_pixel_32 >>  8) & 0xFF) >> 2) << 5)  |  // G
                    ((((*in_pixel_32 >> 16) & 0xFF) >> 3) << 0);    // B
            }

        } else {
            // Convert "RRRRRRRRRGGGGGGGGBBBBBBBB" to "RRRRRGGGGGGBBBBB"
            temp_data    = new unsigned char[image_width * image_height * 2];
            out_pixel_16 = reinterpret_cast<unsigned short *>(temp_data);
            in_pixel_8   = reinterpret_cast<unsigned char *>(image->data());
            
            for (unsigned int i = 0; i < length; ++i) {
                *out_pixel_16++ = 
                    (((*in_pixel_8++ & 0xFF) >> 3) << 11) |  // R
                    (((*in_pixel_8++ & 0xFF) >> 2) << 5)  |  // G
                    (((*in_pixel_8++ & 0xFF) >> 3) << 0);    // B
            }
        }    

    } else if (pixel_format == kPixelFormat_RGBA4444) {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"
        
        in_pixel_32  = reinterpret_cast<unsigned int *>(image->data());
        temp_data    = new unsigned char[image_width * image_height * 2];
        out_pixel_16 = reinterpret_cast<unsigned short *>(temp_data);
        
        for(unsigned int i = 0; i < length; ++i, ++in_pixel_32) {
            *out_pixel_16++ =
                ((((*in_pixel_32 >> 0) & 0xFF) >> 4) << 12) | // R
                ((((*in_pixel_32 >> 8) & 0xFF) >> 4) <<  8) | // G
                ((((*in_pixel_32 >> 16) & 0xFF) >> 4) << 4) | // B
                ((((*in_pixel_32 >> 24) & 0xFF) >> 4) << 0);  // A
        }

    } else if (pixel_format == kPixelFormat_RGB5A1) {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
        in_pixel_32  = reinterpret_cast<unsigned int *>(image->data());
        temp_data    = new unsigned char[image_width * image_height * 2];
        out_pixel_16 = reinterpret_cast<unsigned short *>(temp_data);
        
        for(unsigned int i = 0; i < length; ++i, ++in_pixel_32) {
            *out_pixel_16++ =
                ((((*in_pixel_32 >> 0) & 0xFF) >> 3) << 11) | // R
                ((((*in_pixel_32 >> 8) & 0xFF) >> 3) <<  6) | // G
                ((((*in_pixel_32 >> 16) & 0xFF) >> 3) << 1) | // B
                ((((*in_pixel_32 >> 24) & 0xFF) >> 7) << 0);  // A
        }

    } else if (pixel_format == kPixelFormat_A8) {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "AAAAAAAA"
        in_pixel_32 = reinterpret_cast<unsigned int *>(image->data());
        temp_data = new unsigned char[image_width * image_height];
        unsigned char *out_pixel_8 = temp_data;
        
        for(unsigned int i = 0; i < length; ++i, ++in_pixel_32) {
            *out_pixel_8++ = (*in_pixel_32 >> 24) & 0xFF;  // A
        }
    }
    
    if (has_alpha && pixel_format == kPixelFormat_RGB888) {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRRRRGGGGGGGGBBBBBBBB"
        in_pixel_32 = reinterpret_cast<unsigned int *>(image->data());
        temp_data = new unsigned char[image_width * image_height * 3];
        unsigned char *out_pixel_8 = temp_data;
        
        for(unsigned int i = 0; i < length; ++i, ++in_pixel_32) {
            *out_pixel_8++ = (*in_pixel_32 >> 0) & 0xFF; // R
            *out_pixel_8++ = (*in_pixel_32 >> 8) & 0xFF; // G
            *out_pixel_8++ = (*in_pixel_32 >> 16) & 0xFF; // B
        }
    }
    
    bool result = InitWithData(temp_data, pixel_format, ivec2(image_width, image_height), image_size);
    if (temp_data != image->data()) {
        delete [] temp_data;
    }
    has_premultiplied_alpha_ = image->is_premultipled_alpha();
    return result;
}

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
        S2ERROR("bitsPerPixelForFormat: %ld, cannot give useful result", (long)format);
        assert(false);
        break;
    }

    return ret;
}

void Texture2D::set_tex_parameters(const GLuint min_filter,
                                   const GLuint map_filter,
                                   const GLuint wrap_s,
                                   const GLuint wrap_t) {
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, map_filter );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
}


}


