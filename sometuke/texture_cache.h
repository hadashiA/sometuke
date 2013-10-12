#ifndef __sometuke__texture_cache__
#define __sometuke__texture_cache__

#include <memory>
#include <unordered_map>

namespace sometuke {
using namespace std;

class Texture2D;

class TextureCache {
public:
    static TextureCache& Instance() {
        static unique_ptr<TextureCache> __instance(new TextureCache);
        return *__instance;
    }

    shared_ptr<Texture2D> FetchFromPath(const string& path);
    void Purge() {
        textures_.clear();
    }

private:
    typedef unordered_map<unsigned int, shared_ptr<Texture2D> > TextureTable;

    TextureCache() {}

    TextureCache(const TextureCache&);
    TextureCache& operator=(const TextureCache&);

    TextureTable textures_;
};

}

#endif /* defined(__sometuke__texture_cache__) */



