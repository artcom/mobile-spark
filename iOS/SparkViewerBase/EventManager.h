#import <UIKit/UIKit.h>
#ifdef __cplusplus
    #include "spark/BaseApp.h"
#endif


@interface EventManager : NSObject {
    @private
        UIView *_myView;
        #ifdef __cplusplus
            spark::BaseApp *_myApp;
        #endif
        float _myHeight;
    
}

- (id) initWithSourceView:(UIView*)view targetApp:( 
    #ifdef __cplusplus 
        spark::BaseApp*
    #endif
    )app;
- (void) createTouchRecognizers;
- (void)throwEventToSpark:(NSString*) eventString;




@end