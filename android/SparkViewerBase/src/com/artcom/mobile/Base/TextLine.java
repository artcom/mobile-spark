// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

package com.artcom.mobile.Base;

import android.content.Context;

public class TextLine {
    public String _myLineOfText;
    public int _myXPos;
    public int _myYPos;
    public int _myWidth;
    public TextLine(String myTextLine, int myXPos, int myYPos, int theWidth) {
        _myLineOfText = myTextLine;
        _myXPos = myXPos;
        _myYPos = myYPos;
        _myWidth = theWidth;
    }

}
