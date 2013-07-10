#include "skidarak/texture_cache.h"

#include "skidarak/texture_2d.h"
#include "skidarak/hashed_string.h"
#include "skidarak/application.h"
#include "skidarak/assets_loader.h"

namespace skidarak {

shared_ptr<Texture2D> TextureCache::FetchFromPath(const string& path) {
    unsigned int texture_id = HashedString(path).id();
    if (textures_.find(texture_id) == textures_.end()) {
        shared_ptr<Texture2D> texture = Application::Instance().loader().ReadTexture(path);
        if (texture) {
            textures_[texture_id] = texture;
        } else {
            return texture;
        }
    }

    return textures_[texture_id];
}

}
