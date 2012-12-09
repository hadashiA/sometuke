#include "texture_2d.h"

namespace kawaii {

Texture2D::Texture2D() : id_(0), pixel_size_(0, 0), content_size_(0, 0),
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
}

}
