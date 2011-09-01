#import "EventManager.h"

@implementation EventManager


- (id) initWithSourceView:(UIView*)view targetApp:(spark::BaseApp*)app {
    _myView = view;
    _myApp = app;
    _myHeight = _myView.frame.size.height;
    [self createTouchRecognizers];
    return self;
}



- (void)createTouchRecognizers {
    UIGestureRecognizer *recognizer;
    //single tap
    recognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleSingleTap:)];
    [(UITapGestureRecognizer *)recognizer setNumberOfTouchesRequired:1];
    [_myView addGestureRecognizer:recognizer];
    [recognizer release]; 
    // double tap
    recognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleDoubleTap:)];
    [(UITapGestureRecognizer *)recognizer setNumberOfTapsRequired:2];
    [_myView addGestureRecognizer:recognizer];
    [recognizer release]; 
    // long pressed
    recognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongPressed:)];
    [_myView addGestureRecognizer:recognizer];
    [recognizer release];
    // pan gesture
    recognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePanGesture:)];
    [_myView addGestureRecognizer:recognizer];
    [recognizer release];
    // pinch (e.g. for zoom)
    recognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(handlePinchGesture:)];
    [_myView addGestureRecognizer:recognizer];
    [recognizer release];
    // 2-Finger-Rotation
    recognizer = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(handleRotationGesture:)];
    [_myView addGestureRecognizer:recognizer];
    [recognizer release];
    // swipe left
    recognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeLeftGesture:)];
    [(UISwipeGestureRecognizer *)recognizer setDirection:UISwipeGestureRecognizerDirectionLeft];
    [_myView addGestureRecognizer:recognizer];
    [recognizer release];
    // swipe right
    recognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeRightGesture:)];
    [(UISwipeGestureRecognizer *)recognizer setDirection:UISwipeGestureRecognizerDirectionRight];
    [_myView addGestureRecognizer:recognizer];
    [recognizer release];
}


- (void)handleSingleTap:(UITapGestureRecognizer *)recognizer {
    CGPoint location = [recognizer locationInView:_myView];
    NSLog(@"Touched on  :   %f, %f", location.x, location.y);
    [self throwEventToSpark:[NSString stringWithFormat:@"<TouchEvent type='tap' x='%f' y='%f'/>", location.x, _myHeight-location.y]];
}

- (void)handleDoubleTap:(UITapGestureRecognizer *)recognizer {
    CGPoint location = [recognizer locationInView:_myView];
    NSLog(@"Double-Touch on  :   %f, %f", location.x, location.y);
    [self throwEventToSpark:[NSString stringWithFormat:@"<TouchEvent type='doubleTap' x='%f' y='%f'/>", location.x, _myHeight-location.y]];
}


- (void)handleLongPressed:(UIGestureRecognizer *)recognizer {
    CGPoint location = [recognizer locationInView:_myView];
    NSLog(@"long Pressed  :   %f, %f", location.x, location.y);
    [self throwEventToSpark:[NSString stringWithFormat:@"<TouchEvent type='longPressed' x='%f' y='%f'/>", location.x, _myHeight-location.y]];
}

- (void)handlePanGesture:(UIPanGestureRecognizer *)recognizer {
    CGPoint translation = [recognizer translationInView:_myView];
    CGPoint location = [recognizer locationInView:_myView];
    NSLog(@"Pan-Translation  :   started at: %f, %f   translation:  %f, %f", location.x, location.y, translation.x, translation.y);
    [self throwEventToSpark:[NSString stringWithFormat:@"<GestureEvent type='pan' x='%f' y='%f' dx='f' dy='f'/>", location.x, _myHeight-location.y, translation.x, _myHeight-translation.y]];
}


- (void)handlePinchGesture:(UIGestureRecognizer *)recognizer {
    CGFloat factor = [(UIPinchGestureRecognizer *)recognizer scale];
    NSLog(@"Pinch-Zoom-Scale :   %f", factor);
    [self throwEventToSpark:[NSString stringWithFormat:@"<GestureEvent type='pinch' scale='%f'/>", factor]];
}

- (void)handleRotationGesture:(UIRotationGestureRecognizer *)recognizer {
    CGFloat rotation = [recognizer rotation] *-1;
    NSLog(@"Rotation   :  %f", rotation);
    [self throwEventToSpark:[NSString stringWithFormat:@"<GestureEvent type='rotation' angle='%f'/>", rotation]];
}


- (void)handleSwipeLeftGesture:(UIGestureRecognizer *)recognizer {
    NSLog(@"Swipe Left");
    [self throwEventToSpark:@"<GestureEvent type='swipe' direction='left'/>"];
}


- (void)handleSwipeRightGesture:(UIGestureRecognizer *)recognizer {
    NSLog(@"Swipe Right");
    [self throwEventToSpark:@"<GestureEvent type='swipe' direction='right'/>"];
}


- (void)throwEventToSpark:(NSString*) eventString {
    _myApp->onEvent([eventString UTF8String]);
}


@end
