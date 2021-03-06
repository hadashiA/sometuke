#import "S2ApplicationView.h"
#import "OpenGL_Internal.h"

#import "sometuke/director.h"
#import "sometuke/logger.h"
#import "sometuke/touch_dispatcher.h"

@interface S2ApplicationView (Private)
- (unsigned int)convertPixelFormat:(NSString *)pixelFormat;
- (void)calculateDeltaTime;
- (void)touchDelegate:(NSSet *)touches phase:(sometuke::TouchPhase)phase;
@end

@implementation S2ApplicationView
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

    return [[[self class] alloc] initWithFrame:frame
                                    colorFormat:format
                                    depthFormat:depth
                             preserveBackbuffer:preserveBackbuffer
                                     sharegroup:sharegroup
                                  multiSampling:sampling
                               numberOfSamples:nSamples];
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

        S2INFO("GL_VENDOR:   %s", glGetString(GL_VENDOR));
        S2INFO("GL_RENDERER: %s", glGetString(GL_RENDERER));
        S2INFO("GL_VERSION:  %s", glGetString(GL_VERSION));

        supportsDiscardFramebuffer_ = [self checkForGLExtension:@"GL_EXT_discard_framebuffer"];

        nextDeltaTimeZero_ = YES;
        lastDisplayTime_ = 0;

        isAnimating_ = NO;

        CGSize size = self.bounds.size;
        float scale = [UIScreen mainScreen].scale;

        sometuke::Director& director = sometuke::Director::Instance();
        director.set_content_scale_factor(scale);
        director.Resize(size.width, size.height);

        motionManager_ = [[CMMotionManager alloc] init];
        if (motionManager_.accelerometerAvailable) {
            [motionManager_ startAccelerometerUpdates];
        }

        // [self mainLoop:nil];
    }
    return self;
}

- (BOOL)resizeFromLayer {
    CAEAGLLayer *layer = (CAEAGLLayer *)self.layer;

    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);
    if (![context_ renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer]) {
        S2ERROR("failed to call context");
    }

    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH,
                                 &backingWidth_);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT,
                                 &backingHeight_);
    S2INFO("surface size:%dx%d", backingWidth_, backingHeight_);

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
            S2ERROR("Failed to make complete framebuffer object 0x%X", error);
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
        S2ERROR("Failed to make complete framebuffer object 0x%X", error);
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
        S2ERROR("Failed to swap renderbuffer in %s\n", __FUNCTION__);
    
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
    sometuke::Director::Instance().Resize(backingWidth_, backingHeight_);

    // [self mainLoop:nil];
}

#pragma mark -
#pragma mark IIGameLoop Protocol

- (void)mainLoop:(CADisplayLink *)displayLink {
    [EAGLContext setCurrentContext:context_];

    [self calculateDeltaTime];
    if (motionManager_.accelerometerActive) {
        //CMAccelerometerData *data = motionManager_.accelerometerData;
        //sometuke::vec2 velocity = sometuke::vec2(data.acceleration.x, data.acceleration.y);
        //sometuke::Director::Instance().dispatcher().Queue<sometuke::AccelerationEvent>(velocity);
    }

    sometuke::Director::Instance().MainLoop(dt_);
    [self swapBuffers];
}

- (void)startAnimation {
    if (isAnimating_) {
        return;
    }

    int frameInterval = (int)floor(sometuke::Director::Instance().animation_interval() * 60.0f);
    displayLink_ = [CADisplayLink displayLinkWithTarget:self selector:@selector(mainLoop:)];
    [displayLink_ setFrameInterval:frameInterval];

    [displayLink_ addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    isAnimating_ = YES;
}

- (void)stopAnimation {
    if (!isAnimating_)
        return;

// #if 0
//     [runningThread_ cancel];
//     [runningThread_ release];
//     runningThread_ = nil;
// #endif

    [displayLink_ invalidate];
    displayLink_ = nil;
    isAnimating_ = NO;
}

- (void)pause {
    sometuke::Director::Instance().Pause();
}

- (void)resume {
    sometuke::Director::Instance().Resume();
}

#pragma mark -
#pragma mark Pass the touches to the superview

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    [self touchDelegate:touches phase:sometuke::kTouchBegan];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    [self touchDelegate:touches phase:sometuke::kTouchMoved];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    [self touchDelegate:touches phase:sometuke::kTouchEnded];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    [self touchDelegate:touches phase:sometuke::kTouchCancelled];
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

- (void)touchDelegate:(NSSet *)touches phase:(sometuke::TouchPhase)phase {
    sometuke::TouchDispatcher& dispatcher = sometuke::TouchDispatcher::Instance();

    if (dispatcher.enabled()) {
        std::vector<std::shared_ptr<sometuke::Touch> > touches_vec;
        for (UITouch *touch in touches) {
            CGPoint pos  = [touch locationInView:self];
            CGPoint prev = [touch previousLocationInView:self];

            std::shared_ptr<sometuke::Touch> k_touch =
                sometuke::Pool<sometuke::Touch>((sometuke::TouchId)touch,
                                                phase,
                                                sometuke::vec2(pos.x, pos.y),
                                                sometuke::vec2(prev.x, prev.y),
                                                touch.tapCount);
            touches_vec.push_back(k_touch);
        }
        dispatcher.Trigger(phase, touches_vec);
    }
}
@end
