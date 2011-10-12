#import <CoreMotion/CoreMotion.h>

#import <UIKit/UIKit.h>

@interface Sensors : NSObject{
    @private
        CMMotionManager * motionManager;

}

- (id) init;
- (void) enable;
- (void) disable;
- (void)throwEventToSpark:(NSString*) eventString;




@end