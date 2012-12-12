#import "EAGLView.h"
#import "OpenGL_Internal.h"

#import "logger.h"

@interface EAGLView (Private)
- (unsigned int)convertPixelFormat:(NSString *)pixelFormat;
- (void)calculateDeltaTime;
@end

@implementation EAGLView
@synthesize
    backingWidth=backingWidth_,
    backingHeight=backingHeight_,
    eaglColorFormat=eaglColorFormat_,
    pixelFormat=pixelFormat_,
    depthFormat=depthFormat_,
    context=context_,
    surfaceSize=surfaceSize_,
    multiSampling=multiSampling_;

+ (Class)layerClass {
    return [CAEAGLLayer class];
}
      
+ (id)viewWithFrame:(CGRect)frame
        colorFormat:(NSString *)format
        depthFormat:(GLuint)depth
 preserveBackbuffer:(BOOL)preserveBackbuffer
         sharegroup:(EAGLSharegroup *)sharegroup
      multiSampling:(BOOL)sampling
    numberOfSamples:(unsigned int)nSamples {

    return [[[[self class] alloc] initWithFrame:frame
                                   colorFormat:format
                                   depthFormat:depth
                            preserveBackbuffer:preserveBackbuffer
                                    sharegroup:sharegroup
                                 multiSampling:sampling
                               numberOfSamples:nSamples] autorelease];
}

- (id)initWithFrame:(CGRect)frame
        colorFormat:(NSString *)format
        depthFormat:(GLuint)depth
 preserveBackbuffer:(BOOL)preserveBackbuffer
         sharegroup:(EAGLSharegroup *)sharegroup
      multiSampling:(BOOL)sampling
    numberOfSamples:(unsigned int)nSamples {
    self = [super initWithFrame:frame];
    if (self) {
        isUseUITextField_  = YES;
        eaglColorFormat_   = format;
        pixelFormat_       = [self convertPixelFormat:format];
        depthFormat_       = depth;
        multiSampling_     = sampling;
        requestedSamples_  = nSamples;
        markedText_        = nil;

        msaaColorbuffer_ = 0;
        depthBuffer_ = 0;

        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties =
            [NSDictionary dictionaryWithObjectsAndKeys:
                              [NSNumber numberWithBool:preserveBackbuffer], kEAGLDrawablePropertyRetainedBacking,
                          eaglColorFormat_, kEAGLDrawablePropertyColorFormat,
                          nil];

        if (sharegroup) {
            context_ = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2
                                             sharegroup:sharegroup];
        } else {
            context_ = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        }
        if (!context_ || ![EAGLContext setCurrentContext:context_]) {
            [self release];
            return nil;
        }

        glGenFramebuffers(1, &defaultFramebuffer_);
        NSAssert(defaultFramebuffer_, @"Cannt create default frame buffer");

        glGenRenderbuffers(1, &colorRenderbuffer_);
        NSAssert(colorRenderbuffer_, @"Cannt create default render buffer");

        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer_);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  GL_COLOR_ATTACHMENT0,
                                  GL_RENDERBUFFER,
                                  colorRenderbuffer_);

        if (multiSampling_) {
            GLint maxSamplesAllowed;
            glGetIntegerv(GL_MAX_SAMPLES_APPLE, &maxSamplesAllowed);
            samplesToUse_ = MIN(maxSamplesAllowed, requestedSamples_);

            glGenFramebuffers(1, &msaaFramebuffer_);
            NSAssert(msaaFramebuffer_, @"Cannt create default MSAA frame buffer");
            glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer_);
        }

        IIINFO("GL_VENDOR:   %s", glGetString(GL_VENDOR));
        IIINFO("GL_RENDERER: %s", glGetString(GL_RENDERER));
        IIINFO("GL_VERSION:  %s", glGetString(GL_VERSION));

        supportsDiscardFramebuffer_ = [self checkForGLExtension:@"GL_EXT_discard_framebuffer"];

        nextDeltaTimeZero_ = YES;
        lastDisplayTime_ = 0;

        isAnimating_ = NO;

        CGSize size = self.bounds.size;
        backingWidth_  = size.width;
        backingHeight_ = size.height;
        kawaii::Director::Current()->SetProjection(new kawaii::IOSSurface(self));

        [self mainLoop:nil];
    }
    return self;
}

- (BOOL)resizeFromLayer {
    CAEAGLLayer *layer = (CAEAGLLayer *)self.layer;

    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);
    if (![context_ renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer]) {
        IIERROR("failed to call context");
    }

    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH,
                                 &backingWidth_);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT,
                                 &backingHeight_);
    IIINFO("surface size:%dx%d", backingWidth_, backingHeight_);

    if (multiSampling_) {
        if (msaaColorbuffer_) {
            glDeleteRenderbuffers(1, &msaaColorbuffer_);
            msaaColorbuffer_ = 0;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer_);
        glGenRenderbuffers(1, &msaaColorbuffer_);
        NSAssert(msaaColorbuffer_, @"Cannt create default MSAA frame buffer");
        glBindRenderbuffer(GL_RENDERBUFFER, msaaColorbuffer_);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER,
                                              samplesToUse_,
                                              pixelFormat_,
                                              backingWidth_, backingHeight_);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  GL_RENDERBUFFER, msaaColorbuffer_);
        GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (error != GL_FRAMEBUFFER_COMPLETE) {
            IIERROR("Failed to make complete framebuffer object 0x%X", error);
            return NO;
        }
    }
    CHECK_GL_ERROR();

    if (depthFormat_) {
        if (!depthBuffer_) {
            glGenRenderbuffers(1, &depthBuffer_);
            NSAssert(depthBuffer_, @"Cannt create depth buffer");
        }
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer_);
        if (multiSampling_) {
            glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER,
                                                  samplesToUse_,
                                                  depthFormat_,
                                                  backingWidth_,
                                                  backingHeight_);
        } else {
            glRenderbufferStorage(GL_RENDERBUFFER, depthFormat_,
                                  backingWidth_, backingHeight_);
        }
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, depthBuffer_);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);
    }
    CHECK_GL_ERROR();
    
    GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (error != GL_FRAMEBUFFER_COMPLETE) {
        IIERROR("Failed to make complete framebuffer object 0x%X", error);
        return NO;
    }

    return YES;
}

- (void)swapBuffers {
    if (multiSampling_) {
        /* Resolve from msaaFramebuffer to resolveFramebuffer */
        //glDisable(GL_SCISSOR_TEST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, msaaFramebuffer_);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, defaultFramebuffer_);
        glResolveMultisampleFramebufferAPPLE();
    }

    if(supportsDiscardFramebuffer_) {
        if (multiSampling_) {
            if (depthFormat_) {
                GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
                glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, attachments);
            } else {
                GLenum attachments[] = {GL_COLOR_ATTACHMENT0};
                glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, attachments);
            }
            
            glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);
            
        } else if (depthFormat_ ) {
            GLenum attachments[] = { GL_DEPTH_ATTACHMENT};
            glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, attachments);
        }
    }

    if(![context_ presentRenderbuffer:GL_RENDERBUFFER])
        IIERROR("Failed to swap renderbuffer in %s\n", __FUNCTION__);
    
    // We can safely re-bind the framebuffer here, since this will be the
    // 1st instruction of the new main loop
    if( multiSampling_ )
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer_);
    
    CHECK_GL_ERROR();
}

- (BOOL)checkForGLExtension:(NSString *)searchName {
    char *glExtensions = (char *)glGetString(GL_EXTENSIONS);

    // For best results, extensionsNames should be stored in your renderer so that it does not
    // need to be recreated on each invocation.
    NSString *extensionsString = [NSString stringWithCString:glExtensions
                                                    encoding:NSASCIIStringEncoding];
    NSArray *extensionsNames = [extensionsString componentsSeparatedByString:@" "];
    return [extensionsNames containsObject: searchName];
}

#pragma mark -
#pragma mark UIView callbacks

- (void)layoutSubviews {
    [self resizeFromLayer];
    kawaii::Director::Current()->SetProjection(new kawaii::IOSSurface(self));

    [self mainLoop:nil];
}

#pragma mark -
#pragma mark PPGameLoop Protocol

- (void)mainLoop:(CADisplayLink *)displayLink {
    [EAGLContext setCurrentContext:context_];

    [self calculateDeltaTime];

    kawaii::Director::Current()->DrawScene(dt_);

    [self swapBuffers];
}

- (void)startAnimation {
    if (isAnimating_) {
        return;
    }

    int frameInterval = (int)floor(kawaii::Director::Current()->animation_interval() * 60.0f);
    displayLink_ = [CADisplayLink displayLinkWithTarget:self selector:@selector(mainLoop:)];
    [displayLink_ setFrameInterval:frameInterval];

    [displayLink_ addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    isAnimating_ = YES;
}

- (void)stopAnimation {
    if (!isAnimating_) {
        return;
    }
}

#pragma mark -
#pragma mark Private Methods

- (unsigned int)convertPixelFormat:(NSString *)pixelFormat {
    if ([pixelFormat isEqualToString:kEAGLColorFormatRGB565]) {
        return GL_RGB565;
    } else {
        return GL_RGBA8_OES;
    }
}

- (void)calculateDeltaTime {
    if (nextDeltaTimeZero_ || lastDisplayTime_ == 0) {
        dt_ = 0;
        nextDeltaTimeZero_ = NO;
    } else {
        dt_ = displayLink_.timestamp - lastDisplayTime_;
        dt_ = MAX(0, dt_);
    }
    lastDisplayTime_ = displayLink_.timestamp;
}

#pragma mark -
#pragma mark Memory Managements

- (void)dealloc {
    [super dealloc];
}
@end
