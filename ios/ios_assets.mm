#include "ios_assets.h"
#include "logger.h"
#include "texture_2d.h"

#include <UIKit/UIKit.h>
#include <fstream>

namespace kawaii {

const DeviceType IOSAssets::RunningDevice() {
    static DeviceType __running_device = kDeviceNone;
    if (__running_device == kDeviceNone) {
        float content_scale_factor = [[UIScreen mainScreen] scale];

        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            __running_device = (content_scale_factor == 2 ? kiPadRetinaDisplay : kiPad);
        } else {
            __running_device = (content_scale_factor == 2 ? kiPhoneRetinaDisplay : kiPhone);
        }
    }
    return __running_device;
}

const string IOSAssets::FullPathFromRelativePath(const string& relative_path) {
    DeviceType d = RunningDevice();

    string suffix;
    switch (d) {
    case kiPadRetinaDisplay:
        suffix = IPAD_RETINA_DISPLAY_SUFFIX;
        break;
    case kiPad:
        suffix = IPAD_SUFFIX;
        break;
    case kiPhoneRetinaDisplay:
        suffix = IPHONE_RETINA_DISPLAY_SUFFIX;
        break;
    default:
        suffix = "";
        break;
    }
        
    int ext_pos = relative_path.find_last_of('.');
    string extname  = (ext_pos == string::npos ? "" : relative_path.substr(ext_pos));

    string relative_path_with_suffix = relative_path;
    
    if (!suffix.empty() && extname != ".frag" && extname != ".vert") {
        string basename = relative_path.substr(0, ext_pos);

        if (basename.find(suffix) == string::npos) {
            if (extname == ".ccz" || extname == ".gz") {
                ext_pos = basename.find_last_of('.');
                if (ext_pos != string::npos) {
                    extname.insert(0, basename.substr(ext_pos));
                    basename.erase(ext_pos);
                }
            }
            relative_path_with_suffix = basename + suffix + extname;

        } else {
            IIWARN("filename:%s already has the suffix %s. Using it.",
                    relative_path.c_str(), suffix.c_str());
        }
    }
    int dir_pos = relative_path_with_suffix.find_last_of('/');
    string dirname, filename;
    if (dir_pos == string::npos) {
        dirname  = "";
        filename = relative_path_with_suffix;
    } else {
        dirname  = relative_path_with_suffix.substr(0, dir_pos);
        filename = relative_path_with_suffix.substr(dir_pos);
    }

    NSString *result_ns =
        [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:filename.c_str()]
                                        ofType:nil
                                   inDirectory:[NSString stringWithUTF8String:dirname.c_str()]];

    if (!result_ns) {
        IIWARN("file_utils: file not found: %s", relative_path_with_suffix.c_str());
        return string("");
    } else {
        return string([result_ns UTF8String]);
    }
}

vector<char> IOSAssets::ReadBytes(const string &relative_path) {
    ifstream io(FullPathFromRelativePath(relative_path));
    size_t size = io.seekg(0, ios::end).tellg();
    vector<char> buf(size);
    io.seekg(0, ios::beg).read(&buf[0], size);
    io.close();

    return buf;
}

shared_ptr<Texture2D> IOSAssets::ReadTexture(const string &relative_path) {
    string full_path = FullPathFromRelativePath(relative_path);
    NSString *full_path_ns = [NSString stringWithUTF8String:full_path.c_str()];
    UIImage *image = [[UIImage alloc] initWithContentsOfFile:full_path_ns];

    CGImageRef cg_image = image.CGImage;
    if (cg_image == NULL) {
        IIERROR("Can't create Texture. cgImage is NULL");
        return NULL;
    }

    CGImageAlphaInfo info = CGImageGetAlphaInfo(cg_image);

    unsigned int texture_width, texture_height;
    CGContextRef context = NULL;
    void *data = NULL;
    CGColorSpaceRef color_space;
    void *temp_data;
    unsigned int *in_pixel_32;
    unsigned short *out_pixel_16;
    bool has_alpha;
    float content_width, content_height;
    Texture2D::PixelFormat pixel_format;

    

    [image release];

    return NULL;
}

string IOSAssets::ReadString(const string &relative_path) {
    vector<char> buf = ReadBytes(relative_path);
    return string(buf.begin(), buf.end());
}

}
