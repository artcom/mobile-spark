#import "EventManager.h"
#include <spark/AppProvider.h>

@implementation EventManager


- (id) initWithSourceView:(UIView*)view {
    _myView = view;
    _myHeight = _myView.frame.size.height;
    [self createTouchRecognizers];
    recognizer.delegate = self;
    recognizerSwipe.delegate = self;
    return self;
}



- (void)createTouchRecognizers {
    //UIGestureRecognizer *recognizer;
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
    // pinch (e.g. for zoom)
    recognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(handlePinchGesture:)];
    [_myView addGestureRecognizer:recognizer];
    [recognizer release];
    // 2-Finger-Rotation
    recognizer = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(handleRotationGesture:)];
    [_myView addGestureRecognizer:recognizer];
    [recognizer release];
    // pan gesture
    recognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePanGesture:)];
    [_myView addGestureRecognizer:recognizer];
    //[recognizer release];
    // swipe left
    recognizerSwipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture:)];
    [(UISwipeGestureRecognizer *)recognizerSwipe setDirection:UISwipeGestureRecognizerDirectionLeft];
    [_myView addGestureRecognizer:recognizerSwipe];
    // swipe right
    recognizerSwipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture:)];
    [(UISwipeGestureRecognizer *)recognizerSwipe setDirection:UISwipeGestureRecognizerDirectionRight];
    [_myView addGestureRecognizer:recognizerSwipe];
    // swipe up
    recognizerSwipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture:)];
    [(UISwipeGestureRecognizer *)recognizerSwipe setDirection:UISwipeGestureRecognizerDirectionUp];
    [_myView addGestureRecognizer:recognizerSwipe];
    // swipe down
    recognizerSwipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture:)];
    [(UISwipeGestureRecognizer *)recognizerSwipe setDirection:UISwipeGestureRecognizerDirectionDown];
    [_myView addGestureRecognizer:recognizerSwipe];

}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)recognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)recognizerSwipe {
    return YES;
}

- (void)handleSingleTap:(UITapGestureRecognizer *)theRecognizer {
    CGPoint location = [theRecognizer locationInView:_myView];
    NSLog(@"Touched on  :   %f, %f", location.x, location.y);
    [self throwEventToSpark:[NSString stringWithFormat:@"<TouchEvent type='tap' x='%f' y='%f'/>", location.x, _myHeight-location.y]];
}

- (void)handleDoubleTap:(UITapGestureRecognizer *)theRecognizer {
    CGPoint location = [theRecognizer locationInView:_myView];
    NSLog(@"Double-Touch on  :   %f, %f", location.x, location.y);
    [self throwEventToSpark:[NSString stringWithFormat:@"<TouchEvent type='doubleTap' x='%f' y='%f'/>", location.x, _myHeight-location.y]];
}


- (void)handleLongPressed:(UIGestureRecognizer *)theRecognizer {
    CGPoint location = [theRecognizer locationInView:_myView];
    NSLog(@"long Pressed  :   %f, %f", location.x, location.y);
    [self throwEventToSpark:[NSString stringWithFormat:@"<TouchEvent type='longPressed' x='%f' y='%f'/>", location.x, _myHeight-location.y]];
}

- (void)handlePanGesture:(UIPanGestureRecognizer *)theRecognizer {
    CGPoint translation = [theRecognizer translationInView:_myView];
    CGPoint location = [theRecognizer locationInView:_myView];
    NSLog(@"Pan-Translation  :   started at: %f, %f   translation:  %f, %f", location.x, location.y, translation.x, translation.y);
    [self throwEventToSpark:[NSString stringWithFormat:@"<GestureEvent type='pan' x='%f' y='%f' dx='%f' dy='%f'/>", location.x, _myHeight-location.y, translation.x, _myHeight-translation.y]];
}


- (void)handlePinchGesture:(UIGestureRecognizer *)theRecognizer {
    CGFloat factor = [(UIPinchGestureRecognizer *)recognizer scale];
    NSLog(@"Pinch-Zoom-Scale :   %f", factor);
    [self throwEventToSpark:[NSString stringWithFormat:@"<GestureEvent type='pinch' factor='%f'/>", factor]];
}

- (void)handleRotationGesture:(UIRotationGestureRecognizer *)theRecognizer {
    CGFloat rotation = [theRecognizer rotation] *-1;
    NSLog(@"Rotation   :  %f", rotation);
    [self throwEventToSpark:[NSString stringWithFormat:@"<GestureEvent type='rotation' factor='%f'/>", rotation]];
}


- (void)handleSwipeGesture:(UISwipeGestureRecognizer *)theRecognizer {
    NSString *myDir = [[NSMutableString alloc] initWithString:@""];
    switch(theRecognizer.direction) {
        case UISwipeGestureRecognizerDirectionLeft: 
            myDir=@"left";
            break;
        case UISwipeGestureRecognizerDirectionRight: 
            myDir=@"right";
            break;
        case UISwipeGestureRecognizerDirectionUp: 
            myDir=@"up";
            break;
        case UISwipeGestureRecognizerDirectionDown: 
            myDir=@"down";
            break;
    }
    NSString *eventCall = [NSString  stringWithFormat:@"<GestureEvent type='swipe-%@' direction='%@'/>",myDir,myDir];
    [self throwEventToSpark:eventCall];
    NSLog(@"Swipe %@",myDir);
}

- (void)throwEventToSpark:(NSString*) eventString {
    spark::AppProvider::get().getApp()->onEvent([eventString UTF8String]);
}


- (void)dealloc
{
    [recognizer release];
    
    [recognizerSwipe release];
    
    [super dealloc];
}


@end
