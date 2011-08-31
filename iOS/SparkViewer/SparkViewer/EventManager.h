#import <UIKit/UIKit.h>
#ifdef __cplusplus
    #include <spark/BaseApp.h>
#endif


@interface EventManager : NSObject {
    @private
        spark::BaseApp *myApp;
        UIView *myView;
}

- (id) initWithSourceView:(UIView*)view targetApp:(spark::BaseApp*)app;
- (void) createTouchRecognizers;




@end