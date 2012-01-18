// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

package com.artcom.mobile.Base;

import android.view.MotionEvent;

public class EventManager {
    
    private final int RECENT_ACTION_WAS_DOWN = 0;
    private final int RECENT_ACTION_WAS_PANING = 1;
    private final int A_SECOND_FINGER_TOUCHED = 2;
    private final int PINCH_ZOOM_STARTED = 3;
    private final int ROTATION_STARTED = 4;

    private static EventManager INSTANCE;
    private int mode = 0; // 0 - none; 1-pan; 2-TwofingerTap; 3-pinch; 4-rotate
    private int height;
    private long startTime, lastTapTime;
    private int startX, startY, dx, dy, fingerDistance, fingerDistanceStart, zoomCenterX, zoomCenterY;
    //-------------------------------------------------------------------------
    public static boolean dumpEvent(MotionEvent event) {
        if (INSTANCE != null) return INSTANCE.dumpTouchEvent(event);
        return false;
    }
    //-------------------------------------------------------------------------
    public EventManager() {
        INSTANCE = this;
    }
    //-------------------------------------------------------------------------
    public static void onSizeChanged(int width, int height) {
        if (INSTANCE == null) return;
        INSTANCE.height =height;
    }
    //-------------------------------------------------------------------------
    public boolean dumpTouchEvent(MotionEvent event) {
            dx = (int)event.getX(0)-startX;
            dy = height - (int)event.getY(0)-startY;
            long timeNow  = System.currentTimeMillis();
            switch(event.getAction() & MotionEvent.ACTION_MASK) {
                case MotionEvent.ACTION_DOWN:
                    startTime = System.currentTimeMillis();
                    mode = RECENT_ACTION_WAS_DOWN;
                    startX = (int)event.getX(0);
                    startY = height - (int)event.getY(0);
                    downHandler();
                    break;
                case MotionEvent.ACTION_UP:
                    if (timeNow - startTime < 300) {
                        if (timeNow - lastTapTime<300) {
                            doubleTapHandler();
                            break;
                        }
                        if (mode == RECENT_ACTION_WAS_DOWN) {
                            singleTapHandler();
                            lastTapTime = timeNow;
                            break;
                        }
                        if (mode == RECENT_ACTION_WAS_PANING) {
                            if (dx < -50f) swipeLeftHandler();
                            if (dx > 50f) swipeRightHandler();
                            if (dy < -50f) swipeDownHandler();
                            if (dy > 50f) swipeUpHandler();
                            upHandler();
                            break;
                        }
                    } else if (mode == RECENT_ACTION_WAS_DOWN) {
                        longPressedHandler();
                    }
                    upHandler();
                case MotionEvent.ACTION_MOVE:
                    if (mode == A_SECOND_FINGER_TOUCHED) {
                        startX = (int)event.getX(0);
                        startY = height - (int)event.getY(0);                        
                        fingerDistance = getFingerDistance(event);
                        if (fingerDistance>1 && Math.abs(fingerDistance-fingerDistanceStart)>20) {
                            mode = PINCH_ZOOM_STARTED;
                            break;
                        }
                        if (fingerDistance>1 && getRotation(event)>5f) {
                            mode = ROTATION_STARTED;
                            break;
                        }
                        break;
                    }
                    if (mode == PINCH_ZOOM_STARTED) {
                        fingerDistance = getFingerDistance(event);
                        if(fingerDistance>1) pinchHandler();
                        if(dx*dx + dy*dy > 150) {
                            panHandler();
                        }
                        break;
                    }
                    if (mode == ROTATION_STARTED) {
                        // todo
                        rotationHandler();
                        break;
                    }
                    // only one finger is moving:
                    if(dx*dx + dy*dy < 150) break;
                    mode = RECENT_ACTION_WAS_PANING;                    
                    panHandler();
                    break;
                case MotionEvent.ACTION_POINTER_DOWN:
                    if (mode==A_SECOND_FINGER_TOUCHED) break;
                    fingerDistance = getFingerDistance(event);
                    if ( fingerDistance > 5f) {
                        mode = A_SECOND_FINGER_TOUCHED;
                        fingerDistanceStart = fingerDistance;
                    }
                    break;
            }
            return true;
        }
        //-------------------------------------------------------------------------
        private int getFingerDistance(MotionEvent event) {
            if (event.getPointerCount() <2 ) return 0;
            float dx2 = event.getX(0) - event.getX(1);
            float dy2 = event.getY(0) - event.getY(1);
            zoomCenterX = (int)((event.getX(0) + event.getX(1)) / 2.0); 
            zoomCenterY = height - (int)((event.getY(0) + event.getY(1)) / 2.0); 
            return  (int) Math.sqrt(dx2*dx2 + dy2*dy2);
        }
        //-------------------------------------------------------------------------
        private float getRotation(MotionEvent event) {
            // todo
            return  0;
        }
        // -------------------------------------------------------------------------
	    public void dumbBackButtonEvent() {
		    AC_Log.debug(" ########### backbutton: " + startX + ", " + startY);
		    String myEvent = "<TouchEvent type='backbutton' x='" + startX + "' y='"
				+ startY + "'/>";
		    NativeBinding.onEvent(myEvent);
	    }
        //-------------------------------------------------------------------------
        private void downHandler() {
        	AC_Log.debug(" ########### down: " + startX + ", " + startY);
            String myEvent = "<TouchEvent type='down' x='" + startX + "' y='" + startY + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void upHandler() {
            AC_Log.debug(" ########### up: " + startX + ", " + startY);
            String myEvent = "<TouchEvent type='up' x='" + startX + "' y='" + startY + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void singleTapHandler() {
            AC_Log.debug(" ########### single tap: " + startX + ", " + startY);
            String myEvent = "<TouchEvent type='tap' x='" + startX + "' y='" + startY + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void doubleTapHandler() {
            AC_Log.debug(" ########### double tap: " + startX + ", " + startY);
            String myEvent = "<TouchEvent type='doubletap' x='" + startX + "' y='" + startY + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void longPressedHandler() {
            AC_Log.debug(" ########### longPressed: " + startX + ", " + startY);
            String myEvent = "<TouchEvent type='longpress' x='" + startX + "' y='" + startY + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void panHandler() {
            AC_Log.debug(" ########### pan: " + startX + ", " + startY + " translate: " + dx + ", " + dy);
            String myEvent = "<GestureEvent type='pan' x='" + startX + "' y='" + startY + "' dx='" + dx + "' dy='"+ dy + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void pinchHandler() {
            AC_Log.debug (" ########### pinch: factor: " + (float)fingerDistance/fingerDistanceStart + " center : " + zoomCenterX + "/" + zoomCenterY);
            String myEvent = "<GestureEvent type='pinch' factor='" + (float)(fingerDistance)/fingerDistanceStart +  "' x='" + zoomCenterX + "' y='"+ zoomCenterY +"'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void rotationHandler() {
            //AC_Log.debug(" ########### rotation: " + event.getX() + ", " + event.getY());
        }
        //-------------------------------------------------------------------------
        private void swipeLeftHandler() {
            AC_Log.debug(" ########### swipe left");
            String myEvent = "<GestureEvent type='swipe-left' direction='left'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void swipeRightHandler() {
            AC_Log.debug(" ########### swipe right");
            String myEvent = "<GestureEvent type='swipe-right' direction='right'/>";
            NativeBinding.onEvent(myEvent);
        }
         //-------------------------------------------------------------------------
        private void swipeUpHandler() {
            AC_Log.debug(" ########### swipe up");
            String myEvent = "<GestureEvent type='swipe-up' direction='up'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void swipeDownHandler() {
            AC_Log.debug(" ########### swipe down");
            String myEvent = "<GestureEvent type='swipe-down' direction='down'/>";
            NativeBinding.onEvent(myEvent);
        }
}
