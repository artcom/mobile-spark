// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#import <UIKit/UIKit.h>

@interface EventManager : NSObject<UIGestureRecognizerDelegate> {
    @private
        UIView *_myView;
        float _myHeight;
        float _myRetinaScale;
        UIGestureRecognizer *recognizerPan;
        UIGestureRecognizer *recognizerSwipe;
}

- (void) onResize: (CGRect) newFrame;
- (id) initWithSourceView:(UIView*)view;
- (void) createTouchRecognizers;
- (void) throwEventToSpark:(NSString*) eventString;
- (void) onTouchDown:(UITouch*)event;




@end