#include "sometuke/ios/ios_assets_loader.h"
#include "sometuke/logger.h"
#include "sometuke/texture_2d.h"

#include <UIKit/UIKit.h>
#include <fstream>

namespace sometuke {

const string IOSAssetsLoader::FullPathFromRelativePath(const string& relative_path) {
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
        IIWARN("ios_assets_loader: file not found: %s", relative_path_with_suffix.c_str());
        return string("");
    } else {
        return string([result_ns UTF8String]);
    }
}

vector<char> IOSAssetsLoader::ReadBytes(const string &relative_path) {
    ifstream io(FullPathFromRelativePath(relative_path));
    size_t size = io.seekg(0, ios::end).tellg();
    vector<char> buf(size);
    io.seekg(0, ios::beg).read(&buf[0], size);
    io.close();

    return buf;
}

shared_ptr<Texture2D> IOSAssetsLoader::ReadTexture(const string &relative_path) {
    string full_path = FullPathFromRelativePath(relative_path);
    NSString *full_path_ns = [NSString stringWithUTF8String:full_path.c_str()];
    UIImage *image = [[UIImage alloc] initWithContentsOfFile:full_path_ns];

    CGImageRef cg_image = image.CGImage;
    if (cg_image == NULL) {
        S2ERROR("Can't create Texture. cgImage is NULL");
        return NULL;
    }

    CGImageAlphaInfo info = CGImageGetAlphaInfo(cg_image);

    CGContextRef context = NULL;
    void *data = NULL;
    CGColorSpaceRef color_space;
    void *temp_data;
    unsigned int *in_pixel_32;
    unsigned short *out_pixel_16;
    bool has_alpha;
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
            
            S2INFO("Using RGB565 texture since image has no alpha");
        }
    } else {
        S2INFO("Using A8 texture since image is a mask");
        pixel_format = Texture2D::kPixelFormat_A8;
    }

    ivec2 pixel_size(CGImageGetWidth(cg_image),
                     CGImageGetHeight(cg_image));

    // iOS 5 BUG:
    // If width is not word aligned, convert it to word aligned.
    // http://www.cocos2d-iphone.org/forum/topic/31092
    if (ios_version >= kiOSVersion_5_0) {
        unsigned int bpp = Texture2D::BitsPerPixelForFormat(pixel_format);
        unsigned int bytes = pixel_size.x * bpp / 8;
        		
        // XXX: Should it be 4 or sizeof(int) ??
        unsigned int mod = bytes % 4;
		
        // Not word aligned ?
        if( mod != 0 ) {
            unsigned int needed_bytes = (4 - mod ) / (bpp/8);

            IIWARN("converting size=(%d,%d) to size=(%d,%d) due to iOS 5.x memory BUG. See: http://www.cocos2d-iphone.org/forum/topic/31092", pixel_size.x, pixel_size.y, pixel_size.x + needed_bytes, pixel_size.y);
            pixel_size.x += needed_bytes;
        }
    }

    unsigned int max_texture_size = MaxTextureSize();
    if (pixel_size.y > max_texture_size || pixel_size.x > max_texture_size) {
        IIWARN("cocos2d: WARNING: Image (%lu x %lu) is bigger than the supported %ld x %ld",
                  (long)pixel_size.x, (long)pixel_size.y,
                  (long)max_texture_size, (long)max_texture_size);
        return NULL;
    }

    // Create the bitmap graphics context
    switch (pixel_format) {
    case Texture2D::kPixelFormat_RGBA8888:
    case Texture2D::kPixelFormat_RGBA4444:
    case Texture2D::kPixelFormat_RGB5A1:
    case Texture2D::kPixelFormat_RGB565:
    case Texture2D::kPixelFormat_RGB888:
        color_space = CGColorSpaceCreateDeviceRGB();
        data = malloc(pixel_size.y * pixel_size.x * 4);
        context = CGBitmapContextCreate(data, pixel_size.x, pixel_size.y,
                                        8, 4 * pixel_size.x, color_space,
                                        info | kCGBitmapByteOrder32Big);
        CGColorSpaceRelease(color_space);
        break;
    case Texture2D::kPixelFormat_A8:
        data = malloc(pixel_size.y * pixel_size.x);
        info = kCGImageAlphaOnly;
        context = CGBitmapContextCreate(data, pixel_size.x, pixel_size.y, 8, pixel_size.x, NULL, info);
        break;
    default:
        S2ERROR("Invalid pixel format");
        assert(false);
    }
    
    CGContextClearRect(context, CGRectMake(0, 0, pixel_size.x, pixel_size.y));
    // CGContextTranslateCTM(context, 0, textureHeight - imageSize.height);
    CGContextTranslateCTM(context, 0, 0);
    CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(cg_image), CGImageGetHeight(cg_image)), cg_image);

    // Repack the pixel data into the right format
    if (pixel_format == Texture2D::kPixelFormat_RGB565) {
        //Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
        temp_data = malloc(pixel_size.y * pixel_size.x * 2);
        in_pixel_32  = (unsigned int *)data;
        out_pixel_16 = (unsigned short *)temp_data;
        for (unsigned int i = 0; i < pixel_size.x * pixel_size.y; ++i, ++in_pixel_32)
            *out_pixel_16++ = ((((*in_pixel_32 >> 0) & 0xFF) >> 3) << 11) | ((((*in_pixel_32 >> 8) & 0xFF) >> 2) << 5) | ((((*in_pixel_32 >> 16) & 0xFF) >> 3) << 0);
        free(data);
        data = temp_data;

    } else if (pixel_format == Texture2D::kPixelFormat_RGB888) {
        //Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRRRRGGGGGGGGBBBBBBB"
        temp_data = malloc(pixel_size.y * pixel_size.x * 3);
        char *in_data = (char*)data;
        char *out_data = (char*)temp_data;
        int j=0;
        for (unsigned int i = 0; i < pixel_size.x * pixel_size.y *4; i++) {
            out_data[j++] = in_data[i++];
            out_data[j++] = in_data[i++];
            out_data[j++] = in_data[i++];
        }
        free(data);
        data = temp_data;

    } else if (pixel_format == Texture2D::kPixelFormat_RGBA4444) {
        //Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"
        temp_data = malloc(pixel_size.y * pixel_size.x * 2);
        in_pixel_32  = (unsigned int *)data;
        out_pixel_16 = (unsigned short *)temp_data;
        for (unsigned int i = 0; i < pixel_size.x * pixel_size.y; ++i, ++in_pixel_32)
            *out_pixel_16++ =
                ((((*in_pixel_32 >> 0) & 0xFF) >> 4) << 12) | // R
                ((((*in_pixel_32 >> 8) & 0xFF) >> 4) << 8) | // G
                ((((*in_pixel_32 >> 16) & 0xFF) >> 4) << 4) | // B
                ((((*in_pixel_32 >> 24) & 0xFF) >> 4) << 0); // A

        free(data);
        data = temp_data;

    } else if (pixel_format == Texture2D::kPixelFormat_RGB5A1) {
        //Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
        /*
          Here was a bug.
          When you convert RGBA8888 texture to RGB5A1 texture and then render it on black background, you'll see a "ghost" image as if the texture is still RGBA8888. 
          On background lighter than the pixel color this effect disappers.
          This happens because the old convertion function doesn't premultiply old RGB with new A.
          As Result = sourceRGB + destination*(1-source A), then
          if Destination = 0000, then Result = source. Here comes the ghost!
          We need to check new alpha value first (it may be 1 or 0) and depending on it whether convert RGB values or just set pixel to 0 
        */
        temp_data = malloc(pixel_size.y * pixel_size.x * 2);
        in_pixel_32  = (unsigned int *)data;
        out_pixel_16 = (unsigned short *)temp_data;
        for (unsigned int i = 0; i < pixel_size.x * pixel_size.y; ++i, ++in_pixel_32) {
            if ((*in_pixel_32 >> 31))// A can be 1 or 0
                *out_pixel_16++ =
                    ((((*in_pixel_32 >> 0) & 0xFF) >> 3) << 11) | // R
                    ((((*in_pixel_32 >> 8) & 0xFF) >> 3) << 6) | // G
                    ((((*in_pixel_32 >> 16) & 0xFF) >> 3) << 1) | // B
                    1; // A
            else
                *out_pixel_16++ = 0;
        }
		
        free(data);
        data = temp_data;
    }

    shared_ptr<Texture2D> texture = make_shared<Texture2D>();
    texture->LoadData(data, pixel_format, pixel_size, pixel_size);
    texture->set_has_premultiplied_alpha(info == kCGImageAlphaPremultipliedLast ||
                                         info == kCGImageAlphaPremultipliedFirst);

    CGContextRelease(context);
    free(data);
    
    return texture;
}

string IOSAssetsLoader::ReadString(const string &relative_path) {
    vector<char> buf = ReadBytes(relative_path);
    return string(buf.begin(), buf.end());
}

}
