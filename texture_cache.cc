#include "sometuke/texture_cache.h"

#include "sometuke/texture_2d.h"
#include "sometuke/hashed_string.h"
#include "sometuke/director.h"

namespace sometuke {

shared_ptr<Texture2D> TextureCache::FetchFromPath(const string& path) {
    unsigned int texture_id = HashedString(path).id();
    if (textures_.find(texture_id) == textures_.end()) {
        shared_ptr<Texture2D> texture = Director::Instance().image().CreateTextureFromFile(path);
        if (texture) {
            textures_[texture_id] = texture;
        } else {
            return texture;
        }
    }

    return textures_[texture_id];
}

}
