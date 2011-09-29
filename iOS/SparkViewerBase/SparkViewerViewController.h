#import <UIKit/UIKit.h>



@interface SparkViewerViewController : UIViewController <UITextFieldDelegate>{
    @private
    bool isPortrait;
}


-(void)setPortrait:(bool)thePortrait;

@end