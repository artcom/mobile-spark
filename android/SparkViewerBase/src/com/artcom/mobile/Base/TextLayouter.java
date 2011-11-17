// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

package com.artcom.mobile.Base;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.graphics.Paint;
import android.graphics.Rect;

public class TextLayouter {
    private String _myText;
    private int _myMaxWidth;
    private int _myMaxHeight;
    private int _myCanvasWidth;
    private int _myCanvasHeight;
    private int _myRendereredGlyphIndex;
    public TextLayouter(String theText, int maxWidth, int maxHeight) {
        _myText = theText;
        _myMaxWidth = maxWidth;
        _myMaxHeight = maxHeight;
        _myCanvasHeight = 1;
        _myCanvasWidth = 1;
        _myRendereredGlyphIndex = -1;
    }
    int getRenderedGlyphIndex() { return _myRendereredGlyphIndex; }
    int getCanvasHeight() { return _myCanvasHeight; }
    int getCanvasWidth()  { return _myCanvasWidth;  }

    List<TextLine> createLines(Paint myTextPaint, int theLineHeight) {
        List<TextLine> myResult = new ArrayList<TextLine>();

        int myCharacterOnLine = 0;
        Paint.FontMetrics myMetrics = new Paint.FontMetrics();
        myTextPaint.getFontMetrics(myMetrics);

        int myXPos = 0;
        int myLineWidth = 0;
        int myLineHeight = (int)(myMetrics.bottom - myMetrics.top);
        if (theLineHeight != 0) {
        	myLineHeight = theLineHeight;
        }
        int myBaseLine = (int) (- myMetrics.top);

        _myCanvasWidth = _myMaxWidth;
        _myCanvasHeight = _myMaxHeight;

        // find lines
        _myRendereredGlyphIndex = 0;
        List<Integer> myBlocks = new ArrayList<Integer>();
        int myNewLinePos = _myText.indexOf("\\n");
        int myStartPos = 0;
        do {
            if (myNewLinePos != -1) {
                myBlocks.add(myNewLinePos);
                myStartPos = myNewLinePos+2;
            }
            myNewLinePos = _myText.indexOf("\\n", myStartPos);
        } while(myNewLinePos != -1);
        myBlocks.add(_myText.length());

        int myBlockStartPos = 0;
        for (int i = 0; i <  myBlocks.size(); i++) {
        	
            String myBlock = _myText.substring(myBlockStartPos, myBlocks.get(i));
            myNewLinePos = myBlock.indexOf("\\n");
            if (myNewLinePos != -1) {
                myBlock = myBlock.substring(myNewLinePos + 2, myBlock.length()).trim();
                _myRendereredGlyphIndex+=2;
            }
            myBlockStartPos = myBlocks.get(i);
            Rect myRect = new Rect();
            while (myBlock.length() > 0 && (_myMaxHeight == 0 || myBaseLine < _myCanvasHeight)) {
                myLineWidth = 0;
                if (_myMaxWidth == 0)  {
                    myCharacterOnLine = myBlock.length();
                    myTextPaint.getTextBounds(myBlock, 0, myCharacterOnLine, myRect);
                    _myCanvasWidth = Math.max(_myCanvasWidth, myRect.right);
                    myLineWidth = _myCanvasWidth;
                } else {
                    myCharacterOnLine = myTextPaint.breakText(myBlock, 0, myBlock.length(), true, _myMaxWidth, null);//myWidths);
                }
                // if we are not done yet, find last space on and left of pos myCharacterOnLine
                if ( myCharacterOnLine <= myBlock.length()-1) {
                    int myWhitespacePos = myBlock.substring(0, myCharacterOnLine).lastIndexOf(" ");
                    if (myWhitespacePos != -1) {
                        myCharacterOnLine = myWhitespacePos;
                    }
                }
                String myCurrentString = myBlock.substring(0, myCharacterOnLine).trim();
                myBlock = myBlock.substring(myCharacterOnLine, myBlock.length());//.trim();
                // do we have the width already?
                if (myLineWidth == 0 || _myMaxHeight == 0) {
                    myTextPaint.getTextBounds(myCurrentString, 0, myCurrentString.length(), myRect);
                }
                
                if (myLineWidth == 0) {
                    myLineWidth = myRect.right;
                }
                myResult.add(new TextLine(myCurrentString, myXPos, myBaseLine, myLineWidth));
                _myRendereredGlyphIndex += myCharacterOnLine;//.length();

                if (_myMaxHeight == 0) {
                    _myCanvasHeight = myBaseLine + (int)myMetrics.bottom;
                }
                myBaseLine += myLineHeight;
                
            }
            if (_myMaxHeight != 0 && myBaseLine > _myCanvasHeight) {
            	break;
            } else {
                if (i == myBlocks.size()-1) {
                	_myRendereredGlyphIndex = -1;
                }
            }
        }
        return myResult;
    }
}
