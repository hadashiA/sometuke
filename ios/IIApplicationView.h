#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <QuartzCore/QuartzCore.h>
#import <CoreFoundation/CoreFoundation.h>
#import <CoreMotion/CoreMotion.h>

#import "IIProtocols.h"
#import "skidarake/application.h"

// @interface IIApplicationView : UIView <UIKeyInput, UITextInput> {
@interface IIApplicationView : UIView <IIGameLoop> {
    GLint backingWidth_;
    GLint backingHeight_;

    unsigned int samplesToUse_;
    BOOL multiSampling_;
    unsigned int requestedSamples_;

    GLuint depthFormat_;
    NSString *eaglColorFormat_;
    unsigned int pixelFormat_;

    GLuint defaultFramebuffer_;
    GLuint colorRenderbuffer_;
    GLuint depthBuffer_;

    GLuint msaaFramebuffer_;
    GLuint msaaColorbuffer_;

    EAGLContext *context_;

    CGSize surfaceSize_;
    BOOL supportsDiscardFramebuffer_;

    BOOL isUseUITextField_;
    BOOL markedText_;
    CGRect careRect_;
    NSNotification *keyboardShowNotification_;
    BOOL isKeyboardShown_;

    // displayLink
    CADisplayLink *displayLink_;
    BOOL nextDeltaTimeZero_;
    CFTimeInterval lastDisplayTime_;
    float dt_;

    BOOL isAnimating_;

    CMMotionManager *motionManager_;
}

+ (id)viewWithFrame:(CGRect)frame
        colorFormat:(NSString *)format
        depthFormat:(GLuint)depth
 preserveBackbuffer:(BOOL)retained
         sharegroup:(EAGLSharegroup *)sharegroup
      multiSampling:(BOOL)multisampling
    numberOfSamples:(unsigned int)samples;

- (id)initWithFrame:(CGRect)frame
        colorFormat:(NSString *)format
        depthFormat:(GLuint)depth
 preserveBackbuffer:(BOOL)retained
         sharegroup:(EAGLSharegroup *)sharegroup
      multiSampling:(BOOL)multisampling
    numberOfSamples:(unsigned int)samples;

- (BOOL)resizeFromLayer;
- (void)swapBuffers;
- (BOOL)checkForGLExtension:(NSString *)searchName;

@property (nonatomic, readonly) skidarake::Application *application;

@property (nonatomic, readonly) GLint backingWidth;
@property (nonatomic, readonly) GLint backingHeight;
@property (nonatomic, readonly) NSString *eaglColorFormat;
@property (nonatomic, readonly) unsigned int pixelFormat;
@property (nonatomic, readonly) GLuint depthFormat;
@property (nonatomic, readonly) CGSize surfaceSize;
@property (nonatomic, readonly) EAGLContext *context;
@property (nonatomic, assign) BOOL multiSampling;
@end
