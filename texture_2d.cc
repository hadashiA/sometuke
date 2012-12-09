#include "texture_2d.h"

namespace kawaii {

Texture2D::Texture2D() : id_(0), pixelSize_(0, 0), contentSize_(0, 0),
                         format_(kPixelFormat_Default) {
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture2D::LoadData(const void* data, PixelFormat pixelFormat,
                         ivec2 pixelSize, vec2 contentSize) {
    if (pixelFormat == kPixelFormat_RGBA8888) {
        
    }
}

}





