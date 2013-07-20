#import <Foundation/Foundation.h>

@protocol S2GameLoop
- (void)mainLoop:(CADisplayLink *)displayLink;
- (void)startAnimation;
- (void)stopAnimation;
@end
