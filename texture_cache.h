#ifndef __skidarake__texture_cache__
#define __skidarake__texture_cache__

#include <memory>
#include <unordered_map>

namespace skidarake {
using namespace std;

class Texture2D;

class TextureCache {
public:
    static TextureCache& Instance() {
        static unique_ptr<TextureCache> __instance(new TextureCache);
        return *__instance;
    }

    shared_ptr<Texture2D> FetchFromPath(const string& path);

private:
    typedef unordered_map<unsigned int, shared_ptr<Texture2D> > TextureTable;

    TextureCache() {}

    TextureCache(const TextureCache&);
    TextureCache& operator=(const TextureCache&);

    TextureTable textures_;
};

}

#endif /* defined(__skidarake__texture_cache__) */



