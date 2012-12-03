#import <Foundation/Foundation.h>

@protocol IIGameLoop
- (void)mainLoop:(CADisplayLink *)displayLink;
- (void)startAnimation;
- (void)stopAnimation;
@end
