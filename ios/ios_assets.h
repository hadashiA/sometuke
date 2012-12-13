#ifndef __kawaii__ios_assets__
#define __kawaii__ios_assets__

#include "kawaii/assets.h"

namespace kawaii {

using namespace std;

// TODO: Device that separates
typedef enum {
    kiPhone,
    kiPhoneRetinaDisplay,
    kiPad,
    kiPadRetinaDisplay,

    kDeviceNone,
} DeviceType;

static const string IPHONE_RETINA_DISPLAY_SUFFIX = "@x2";
static const string IPAD_SUFFIX                  = "-ipad";
static const string IPAD_RETINA_DISPLAY_SUFFIX   = "-ipad@x2";

class IOSAssets : public Assets {
public:
    vector<char> ReadBytes(const string& relative_path);
    shared_ptr<Texture2D> ReadTexture(const string &relative_path);
    string ReadString(const string& relative_path);

private:
    const DeviceType RunningDevice();
    const string FullPathFromRelativePath(const string& relative_path);

    bool EndsWith(const string& str, const string& suffix) {
        if (str.length() >= suffix.length()) {
            return (0 == str.compare(str.length() - suffix.length(), suffix.length(), suffix));
        } else {
            return false;
        }
    }
};

} 

#endif /* defined(__kawaii__ios_assets__) */
