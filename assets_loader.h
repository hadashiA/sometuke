#ifndef __skidarake__assets_loader__
#define __skidarake__assets_loader__

#include <vector>
#include <memory>
#include <string>

namespace skidarake {
using namespace std;

class Texture2D;

class AssetsLoader {
public:
    virtual ~AssetsLoader() {}
    virtual vector<char> ReadBytes(const string& relative_path) = 0;
    virtual shared_ptr<Texture2D> ReadTexture(const string &relative_path) = 0;
    virtual string ReadString(const string &relative_path) = 0;
    virtual const string FullPathFromRelativePath(const string& relative_path) = 0;
};

}

#endif /* defined(__skidarake__assets_loader__) */
