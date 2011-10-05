#import <UIKit/UIKit.h>

@interface EventManager : NSObject<UIGestureRecognizerDelegate> {
    @private
        UIView *_myView;
        float _myHeight;
        UIGestureRecognizer *recognizerPan;
        UIGestureRecognizer *recognizerSwipe;
}

- (id) initWithSourceView:(UIView*)view;
- (void) createTouchRecognizers;
- (void)throwEventToSpark:(NSString*) eventString;




@end