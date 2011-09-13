package com.artcom.mobile.Base;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.graphics.Paint;
import android.graphics.Rect;

public class TextLayouter {
	private String _myText;
	private int _myFontSize;
	private int _myMaxWidth;
	private int _myMaxHeight;
	private int _myCanvasWidth;
	private int _myCanvasHeight;
    public TextLayouter(String theText, int theFontSize, int maxWidth, int maxHeight) {
    	_myText = theText;
    	_myFontSize = theFontSize;
    	_myMaxWidth = maxWidth;
    	_myMaxHeight = maxHeight;
    }
    int getCanvasHeight() { return _myCanvasHeight; }
    int getCanvasWidth()  { return _myCanvasWidth;  }
    
    List<TextLine> createLines(Paint myTextPaint) {
    	List<TextLine> myResult = new ArrayList<TextLine>();

    	int myCharacterOnLine = 0; 
    	Paint.FontMetrics myMetrics = new Paint.FontMetrics();
    	myTextPaint.getFontMetrics(myMetrics);
    	    	
    	//AC_Log.print(String.format("Java:createLines %s ", _myText));    	
    	//AC_Log.print(String.format("Java:createLines max width %d", _myMaxWidth));
    	int myXPos = 0;
    	int myLineHeight = (int)(myMetrics.bottom - myMetrics.top);
    	int myBaseLine = (int) (- myMetrics.top);

		_myCanvasWidth = _myMaxWidth;
    	_myCanvasHeight = _myMaxHeight;
    	
    	String myBlock = _myText;
    	while (myBlock.length() > 0 && (_myMaxHeight == 0 || myBaseLine < _myCanvasHeight)) {
    		if (_myMaxWidth == 0)  {
    			myCharacterOnLine = myBlock.length();
    			Rect myRect = new Rect(); 
    			myTextPaint.getTextBounds(myBlock, 0, myCharacterOnLine, myRect);
    			_myCanvasWidth = Math.max(_myCanvasWidth, myRect.right);	
    		} else {
    	    	myCharacterOnLine = myTextPaint.breakText(myBlock, 0, myBlock.length(), true, _myMaxWidth, null); 
    		}
	    	//AC_Log.print(String.format("myCharacterOnLine : %d canvasWidth: %d" ,myCharacterOnLine, _myCanvasWidth));
	    	// if we are not done yet, find last space on and left of pos myCharacterOnLine	  
	    	//AC_Log.print(String.format("%d, %d %s:" ,(myCharacterOnLine), myBlock.length()-1, myBlock.substring(0, myCharacterOnLine)));
	    	if ( myCharacterOnLine <= myBlock.length()-1) {
		    	int myWhitespacePos = myBlock.substring(0, myCharacterOnLine).lastIndexOf(" ");
		    	if (myWhitespacePos != -1) {
		    		myCharacterOnLine = myWhitespacePos;
		    	}
	    	}
	    	String myCurrentString = myBlock.substring(0, myCharacterOnLine).trim();
	    	myBlock = myBlock.substring(myCharacterOnLine, myBlock.length()).trim();
	    	//AC_Log.print(String.format("Java:createLines '%s' line #%d, has character #%d '%s' ", myBlock, myResult.size(), myCharacterOnLine, myCurrentString));
	    	myResult.add(new TextLine(myCurrentString, myXPos, myBaseLine));
	    	myBaseLine += myLineHeight;

	    	if (_myMaxHeight == 0) {
	    		_myCanvasHeight = myBaseLine;
	    	}
    		//AC_Log.print(String.format("_myCanvasHeight %d %d", _myCanvasHeight, _myMaxHeight));
    	}
    	return myResult;
    }
}
