#import "EventManager.h"

@implementation EventManager


- (id) initWithSourceView:(UIView*)view targetApp:(spark::BaseApp*)app {
    myView = view;
    myApp = app;
    [self createTouchRecognizers];
    return self;
}



- (void)createTouchRecognizers {
    // single tap
    UITapGestureRecognizer *singleFingerTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleSingleTap:)];
    [myView addGestureRecognizer:singleFingerTap];
    [singleFingerTap release];    
    // double tap
    UITapGestureRecognizer *singleFingerDoubleTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleDoubleTap:)];
    [myView addGestureRecognizer:singleFingerDoubleTap];
    singleFingerDoubleTap.numberOfTapsRequired = 2;
    [singleFingerDoubleTap release];    
    // pan gesture
    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePanGesture:)];
    [myView addGestureRecognizer:panGesture];
    [panGesture release];
    // pinch (e.g. for zoom)
    UIPinchGestureRecognizer *pinchGesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(handlePinchGesture:)];
    [myView addGestureRecognizer:pinchGesture];
    [pinchGesture release];
    // long pressed
    UILongPressGestureRecognizer *longPressed = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongPressed:)];
    [myView addGestureRecognizer:longPressed];
    [longPressed release];
    // 2-Finger-Rotation
    UIRotationGestureRecognizer *rotationGesture = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(handleRotationGesture:)];
    [myView addGestureRecognizer:rotationGesture];
    [rotationGesture release];
    
}


- (void)handleSingleTap:(UITapGestureRecognizer *)recognizer {
    CGPoint location = [recognizer locationInView:myView];
    NSLog(@"Touched on  :   %f, %f", location.x, location.y);
    myApp->onTouch();
}

- (void)handleDoubleTap:(UITapGestureRecognizer *)recognizer {
    CGPoint location = [recognizer locationInView:myView];
    NSLog(@"Double-Touch on  :   %f, %f", location.x, location.y);
    // TODO
}


- (void)handlePanGesture:(UIPanGestureRecognizer *)recognizer {
    CGPoint translation = [recognizer translationInView:myView];
    NSLog(@"Pan-Translation  :   %f, %f", translation.x, translation.y);
    // TODO
}


- (void)handlePinchGesture:(UIGestureRecognizer *)recognizer {
    CGFloat factor = [(UIPinchGestureRecognizer *)recognizer scale];
    NSLog(@"Pinch-Zoom-Scale :   %f", factor);
    // TODO
    myApp->onSizeChanged(768.0f/factor, 1024.f/factor);
}


- (void)handleLongPressed:(UIGestureRecognizer *)recognizer {
    CGPoint location = [recognizer locationInView:myView];
    NSLog(@"long Pressed  :   %f, %f", location.x, location.y);
    // TODO
}

- (void)handleRotationGesture:(UIRotationGestureRecognizer *)recognizer {
    CGFloat rotation = [recognizer rotation] *1;
    NSLog(@"Rotation   :  %f", rotation);
    // TODO
}


@end