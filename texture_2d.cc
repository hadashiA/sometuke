#include "texture_2d.h"

namespace kawaii {

Texture2D::Texture2D() : id_(0), pixelSize_(0, 0), contentSize_(0, 0),
                         format_(kTexture2DPixelFormat_Default) {
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture2D::LoadData(const void* data, Texture2DPixelFormat pixelFormat,
                         ivec2 pixelSize_, vec2 contentSize_) {
    
}

}
