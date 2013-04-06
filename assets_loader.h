#ifndef __kawaii__assets_loader__
#define __kawaii__assets_loader__

#include <vector>
#include <memory>
#include <string>

namespace kawaii {
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

#endif /* defined(__kawaii__assets_loader__) */
