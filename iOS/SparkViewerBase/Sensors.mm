#include <spark/AppProvider.h>

#import "Sensors.h"

@implementation Sensors


- (id) init {
    motionManager = [[CMMotionManager alloc] init];

    return self;
}

- (void) enable {
    
    
    //Gyroscope
    if([motionManager isGyroAvailable] && ![motionManager isGyroActive]) {
        [motionManager setGyroUpdateInterval:1.0f / 20.0f];
        [motionManager startGyroUpdatesToQueue:[NSOperationQueue mainQueue] withHandler:^(CMGyroData *gyroData, NSError *error) {
                NSString *eventCall = [NSString  stringWithFormat:@"<SensorEvent type='GYROSCOPE' value0='%f' value1='%f' value2='%f'/>",gyroData.rotationRate.x, gyroData.rotationRate.y, gyroData.rotationRate.z];
                 [self throwEventToSpark:eventCall];
        }];
        
    } else { 
        NSLog(@"Gyroscope not available or already active");
    }
    
    //Accelorometer
    if([motionManager isAccelerometerAvailable] && ![motionManager isAccelerometerActive]) {
        [motionManager setAccelerometerUpdateInterval:1.0f / 20.0f];
        [motionManager startAccelerometerUpdatesToQueue:[NSOperationQueue mainQueue] withHandler:^(CMAccelerometerData *accelerometerData, NSError *error) {
            NSString *eventCall = [NSString  stringWithFormat:@"<SensorEvent type='ACCELEROMETER' value0='%f' value1='%f' value2='%f'/>",accelerometerData.acceleration.x, accelerometerData.acceleration.y, accelerometerData.acceleration.z];
            [self throwEventToSpark:eventCall];
        }];
        
    } else { 
        NSLog(@"Accelerometer not available or already active");
    }

    
}

- (void)throwEventToSpark:(NSString*) eventString {
    spark::AppProvider::get().getApp()->onEvent([eventString UTF8String]);
}


- (void) disable {
    if (motionManager.isGyroActive) [motionManager stopGyroUpdates];
    if (motionManager.isAccelerometerActive) [motionManager stopAccelerometerUpdates];

}

- (void)dealloc {
    [super dealloc];
    [motionManager release];
}

@end