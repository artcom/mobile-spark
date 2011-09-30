#import <UIKit/UIKit.h>

@interface EventManager : NSObject {
    @private
        UIView *_myView;
        float _myHeight;
    
}

- (id) initWithSourceView:(UIView*)view;
- (void) createTouchRecognizers;
- (void)throwEventToSpark:(NSString*) eventString;




@end