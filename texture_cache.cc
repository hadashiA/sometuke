#include "texture_cache.h"

#include "texture_2d.h"
#include "hashed_string.h"
#include "application.h"
#include "assets.h"

namespace kawaii {

TextureCache *TextureCache::__current = NULL;

TextureCache *TextureCache::Current() {
    if (__current == NULL) {
        __current = new TextureCache;
    }
    return __current;
}

shared_ptr<Texture2D> TextureCache::AddImage(const string& path) {
    unsigned int texture_id = HashedString(path).value();
    if (textures_.find(texture_id) == textures_.end()) {
        shared_ptr<Texture2D> texture = Application::Current()->assets()->ReadTexture(path);
        textures_[texture_id] = texture;
    }

    return textures_[texture_id];
}

}
