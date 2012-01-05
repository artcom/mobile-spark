// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include <spark/AppProvider.h>

#import "Sensors.h"
#import <Math.h>

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
    
    //Orientation
    if([motionManager isDeviceMotionAvailable] && ![motionManager isDeviceMotionActive]) {
        [motionManager setDeviceMotionUpdateInterval:1.0f / 20.0f];
        [motionManager startDeviceMotionUpdatesToQueue:[NSOperationQueue mainQueue] withHandler:^(CMDeviceMotion *deviceMotionData, NSError *error) {
            float myRoll = deviceMotionData.attitude.roll*180/M_PI;
            float myYaw = deviceMotionData.attitude.yaw*180/M_PI;
            float myPitch = deviceMotionData.attitude.pitch*180/M_PI;
            NSString *eventCall = [NSString  stringWithFormat:@"<SensorEvent type='ORIENTATION' value0='%f' value1='%f' value2='%f'/>",myYaw, myPitch, myRoll];
            [self throwEventToSpark:eventCall];
        }];
        
    } else { 
        NSLog(@"Orientation not available or already active");
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