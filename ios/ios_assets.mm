#include "ios_assets.h"
#include "logger.h"
#include "texture_2d.h"

#include <UIKit/UIKit.h>
#include <fstream>

namespace kawaii {

const string IOSAssets::FullPathFromRelativePath(const string& relative_path) {
    DeviceType d = RunningDevice();

    string suffix = suffixes_[d];
        
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

    unsigned int ios_version = RunningVersion();
    if (ios_version >= kiOSVersion_4_0 && ios_version < kiOSVersion_5_0) {
        has_alpha = ((info == kCGImageAlphaNoneSkipLast) ||
                     (info == kCGImageAlphaPremultipliedLast) ||
                     (info == kCGImageAlphaPremultipliedFirst) ||
                     (info == kCGImageAlphaLast) ||
                     (info == kCGImageAlphaFirst));
    } else {
        has_alpha = ((info == kCGImageAlphaPremultipliedLast) ||
                     (info == kCGImageAlphaPremultipliedFirst) ||
                     (info == kCGImageAlphaLast) ||
                     (info == kCGImageAlphaFirst));
    }

    if ((color_space = CGImageGetColorSpace(cg_image))) {
        if (has_alpha) {
            pixel_format = Texture2D::kPixelFormat_Default;
            info = kCGImageAlphaPremultipliedLast;
        } else {
            info = kCGImageAlphaNoneSkipLast;
            
            // Use RGBA8888 if default is RGBA8888, otherwise use RGB565.
            // DO NOT USE RGB888 since it is the same as RGBA8888, but it is more expensive to create it
            if (Texture2D::kPixelFormat_Default == Texture2D::kPixelFormat_RGBA8888 )
                pixel_format = Texture2D::kPixelFormat_RGBA8888;
            else
                pixel_format = Texture2D::kPixelFormat_RGB565;
            
            IIINFO("Using RGB565 texture since image has no alpha");
        }
    } else {
        IIINFO("Using A8 texture since image is a mask");
        pixel_format = Texture2D::kPixelFormat_A8;
    }

    ivec2 pixel_size(CGImageGetWidth(cg_image),
                     CGImageGetHeight(cg_image));

    // iOS 5 BUG:
    // If width is not word aligned, convert it to word aligned.
    // http://www.cocos2d-iphone.org/forum/topic/31092
    if (ios_version >= kiOSVersion_5_0) {
    }

    // unsigned int max_texture_size = 

    [image release];

    return NULL;
}

string IOSAssets::ReadString(const string &relative_path) {
    vector<char> buf = ReadBytes(relative_path);
    return string(buf.begin(), buf.end());
}

}
