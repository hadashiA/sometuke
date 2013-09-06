#include "sometuke/ios/ios_image_loader.h"
#include "sometuke/logger.h"
#include "sometuke/texture_2d.h"

#include <UIKit/UIKit.h>
#include <fstream>

namespace sometuke {

shared_ptr<Texture2D> IOSImageLoader::CreateTextureFromFile(const string& path) {
    string full_path = Director::Instance().file_utils().FullPathFromRelativePath(path);
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

    unsigned int max_texture_size = Director::Instance().configuration().MaxTextureSize();
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
    texture->InitWithData(data, pixel_format, pixel_size, pixel_size);
    texture->set_has_premultiplied_alpha(info == kCGImageAlphaPremultipliedLast ||
                                         info == kCGImageAlphaPremultipliedFirst);

    CGContextRelease(context);
    free(data);
    
    return texture;
}

static CGSize CalculateStringSize(NSString *str, id font, CGSize constrainSize) {
    NSArray *listItems = [str componentsSeparatedByString:@"\n"];
    CGSize dim = CGSizeZero;
    CGSize textRect = CGSizeZero;
    textRect.width  = (constrainSize.width  > 0 ? constrainSize.width  : 0x7fffffff);
    textRect.height = (constrainSize.height > 0 ? constrainSize.height : 0x7fffffff);
    
    for (NSString *s in listItems) {
        CGSize tmp = [s sizeWithFont:font constrainedToSize:textRect];
        if (tmp.width > dim.width) {
            dim.width = tmp.width; 
        }
        dim.height += tmp.height;
    }
    return dim;
}


bool IOSImage::InitWtihFile(const string& file) {
    S2ERROR("not implemented IOSImage::InitWithFile");
    return true;
}
    
bool IOSImage::InitWithText(const string& text, const string& font_name, int size,
                            int width, int height) {
    FontDefinition font_def;

    font_def.shadow.enabled = false;
    font_def.name  = font_name;
    font_def.size  = size;
    font_def.dimensions = ivec2(width, height);

    return InitWithText(text, font_def);
}

bool IOSImage::InitWithText(const string& text_std, const FontDefinition& font_def) {
    // image init
    NSString *text = [NSString stringWithUTF8String:text_std.c_str()];
    NSString *font_name = [NSString stringWithUTF8String:font_def.name.c_str()];
        
    CGSize dim, constrainSize;
    constrainSize.width  = font_def.dimensions.x;
    constrainSize.height = font_def.dimensions.y;
        
    // On iOS custom fonts must be listed beforehand in the App info.plist (in order to be usable) and referenced only the by the font family name itself when
    // calling [UIFont fontWithName]. Therefore even if the developer adds 'SomeFont.ttf' or 'fonts/SomeFont.ttf' to the App .plist, the font must
    // be referenced as 'SomeFont' when calling [UIFont fontWithName]. Hence we strip out the folder path components and the extension here in order to get just
    // the font family name itself. This stripping step is required especially for references to user fonts stored in CCB files; CCB files appear to store
    // the '.ttf' extensions when referring to custom fonts.
    font_name = [[font_name lastPathComponent] stringByDeletingPathExtension];
        
    // create the font   
    id font = [UIFont fontWithName:font_name size:font_def.size];
        
    if (font) {
        dim = CalculateStringSize(text, font, constrainSize);
    } else {
        font = [UIFont systemFontOfSize:font_def.size];
    }

    // compute start point
    int start_h = 0;
    if (constrainSize.height > dim.height) {
        // vertical alignment
        if (font_def.v_alignment == TextVAlignment::TOP) {
            start_h = 0;
        } else if (font_def.v_alignment == TextVAlignment::CENTER) {
            start_h = (constrainSize.height - dim.height) / 2;
        } else {
            start_h = constrainSize.height - dim.height;
        }
    }
    
    // adjust text rect
    if (constrainSize.width > 0 && constrainSize.width > dim.width) {
        dim.width = constrainSize.width;
    } if (constrainSize.height > 0 && constrainSize.height > dim.height) {
        dim.height = constrainSize.height;
    }
        
    // compute the padding needed by shadow and stroke
    float shadow_stroke_padding_x = 0.0f;
    float shadow_stroke_padding_y = 0.0f;
    if (font_def.stroke.enabled) {
        shadow_stroke_padding_x = ceilf(font_def.stroke.size);
        shadow_stroke_padding_y = ceilf(font_def.stroke.size);
    }
        
    if (font_def.shadow.enabled ) {
        shadow_stroke_padding_x = std::max(shadow_stroke_padding_x,
                                           (float)abs(font_def.shadow.offset.x));
        shadow_stroke_padding_y = std::max(shadow_stroke_padding_y,
                                           (float)abs(font_def.shadow.offset.y));
    }
        
    // add the padding (this could be 0 if no shadow and no stroke)
    dim.width  += shadow_stroke_padding_x;
    dim.height += shadow_stroke_padding_y;
        
    size_t data_size    = static_cast<size_t>(dim.width * dim.height * 4);
    unsigned char* data = new unsigned char[data_size];
    memset(data, 0, data_size);
        
    // draw text
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context =
        CGBitmapContextCreate(data,
                              dim.width,
                              dim.height,
                              8,
                              (int)(dim.width) * 4,
                              colorSpace,
                              kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
        
    if (!context) {
        delete[] data;
        S2ERROR("CGBitmapContextCreate failed");
        return false;
    }

    float stroke_color_r = 0.0;
    float stroke_color_g = 0.0;
    float stroke_color_b = 0.0;
    float stroke_size    = 0.0;
    if (font_def.stroke.enabled) {
        stroke_color_r = font_def.stroke.color.r / 255;
        stroke_color_g = font_def.stroke.color.g / 255;
        stroke_color_b = font_def.stroke.color.b / 255;
        stroke_size    = font_def.stroke.size;
    }

    // text color
    CGContextSetRGBFillColor(context, stroke_color_r, stroke_color_g, stroke_color_b, 1);
    // move Y rendering to the top of the image
    CGContextTranslateCTM(context, 0.0f, (dim.height - shadow_stroke_padding_y) );
    CGContextScaleCTM(context, 1.0f, -1.0f); //NOTE: NSString draws in UIKit referential i.e. renders upside-down compared to CGBitmapContext referential
        
    // store the current context
    UIGraphicsPushContext(context);
        
    // measure text size with specified font and determine the rectangle to draw text in
    NSTextAlignment align;
    if (font_def.h_alignment == TextHAlignment::RIGHT) {
        align = NSTextAlignmentRight;
    } else if (font_def.h_alignment == TextHAlignment::CENTER) {
        align = NSTextAlignmentCenter;
    } else {
        align = NSTextAlignmentLeft;
    }
        
    // take care of stroke if needed
    if (font_def.stroke.enabled) {
        CGContextSetTextDrawingMode(context, kCGTextFillStroke);
        CGContextSetRGBStrokeColor(context,
                                   stroke_color_r,
                                   stroke_color_g,
                                   stroke_color_b,
                                   1);
        CGContextSetLineWidth(context, font_def.stroke.size);
    }
        
    // take care of shadow if needed
    if (font_def.shadow.enabled) {
        CGSize offset;
        offset.height = font_def.shadow.offset.y;
        offset.width  = font_def.shadow.offset.x;
        CGContextSetShadow(context, offset, font_def.shadow.blur);
    }
        
    // normal fonts
    //if( [font isKindOfClass:[UIFont class] ] )
    //{
    //    [str drawInRect:CGRectMake(0, startH, dim.width, dim.height) withFont:font lineBreakMode:(UILineBreakMode)UILineBreakModeWordWrap alignment:align];
    //}
    //else // ZFont class
    //{
    //    [FontLabelStringDrawingHelper drawInRect:str rect:CGRectMake(0, startH, dim.width, dim.height) withZFont:font lineBreakMode:(UILineBreakMode)UILineBreakModeWordWrap 
    ////alignment:align];
    //}
        
    // compute the rect used for rendering the text
    // based on wether shadows or stroke are enabled
        
    float text_width    = dim.width  - shadow_stroke_padding_x;
    float text_height   = dim.height - shadow_stroke_padding_y;
        
    float text_origin_x = (font_def.shadow.offset.x < 0 ? shadow_stroke_padding_x : 0);
    float text_origin_y = (font_def.shadow.offset.y > 0 ? start_h : (start_h - shadow_stroke_padding_y));
    // actually draw the text in the context
    // XXX: ios7 casting
    [text drawInRect:CGRectMake(text_origin_x, text_origin_y, text_width, text_height)
            withFont:font
       lineBreakMode:NSLineBreakByWordWrapping
           alignment:(NSTextAlignment)align];
        
    // pop the context
    UIGraphicsPopContext();
        
    // release the context
    CGContextRelease(context);
               
    // output params
    bytes_.assign(data, data + data_size);
    width_  = static_cast<short>(dim.width);
    height_ = static_cast<short>(dim.height);
    has_alpha_ = true;
    pre_multi_ = true;
    bits_per_component_ = 8;
    
    return true;
}

}
