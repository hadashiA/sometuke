#ifndef __kawaii__file_utils__
#define __kawaii__file_utils__

#include <string>
#include <vector>
#include <memory>

namespace kawaii {

using namespace std;

class Texture2D;

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

const DeviceType RunningDevice();
const string FullPathFromRelativePath(const string& relative_path);
shared_ptr<Texture2D> ReadTextureResource(const string &relative_path);
string ReadStringResource(const string& relative_path);

} 

#endif /* defined(__kawaii__file_utils__) */
