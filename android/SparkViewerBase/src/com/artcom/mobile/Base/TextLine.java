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
