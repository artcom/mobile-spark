package com.artcom.mobile.Base;

import android.view.MotionEvent;

public class EventManager {

    private static EventManager INSTANCE;
    private int mode=0; // 0 - none; 1-pan; 2-TwofingerTap; 3-pinch; 4-rotate
    private int height;
    private long startTime, lastTapTime;
    private int startX, startY, dx, dy, fingerDistance, fingerDistanceStart;
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
            dx = (int)event.getX()-startX;
            dy = height - (int)event.getY()-startY;
            long timeNow  = System.currentTimeMillis();
            switch(event.getAction() & MotionEvent.ACTION_MASK) {
                case MotionEvent.ACTION_DOWN:
                    startTime = System.currentTimeMillis();
                    mode=0;
                    startX = (int)event.getX();
                    startY = height - (int)event.getY();
                    downHandler();
                    break;
                case MotionEvent.ACTION_UP:
                    upHandler();
                    if (timeNow - startTime < 400) {
                        if (timeNow - lastTapTime<400) {
                            doubleTapHandler();
                            break;
                        }
                        if (mode == 0) {
                            singleTapHandler();
                            lastTapTime = timeNow;
                            break;
                        }
                        if (mode == 1) {
                            if (dx < -100f) swipeLeftHandler();
                            if (dx > 100f) swipeRightHandler();
                            break;
                        }
                    } else if (mode == 0) {
                        longPressedHandler();
                        break;
                    }
                case MotionEvent.ACTION_MOVE:
                    if (mode == 2) {
                        fingerDistance = getFingerDistance(event);
                        if (fingerDistance>1 && Math.abs(fingerDistance-fingerDistanceStart)>20) {
                            mode = 3;
                            break;
                        }
                        if (fingerDistance>1 && getRotation(event)>5f) {
                            mode = 4;
                            break;
                        }
                        break;
                    }
                    if (mode == 3) {
                        fingerDistance = getFingerDistance(event);
                        if(fingerDistance>1) pinchHandler();
                        break;
                    }
                    if (mode == 4) {
                        // todo
                        rotationHandler();
                        break;
                    }
                    if(dx*dx + dy*dy < 10) break;
                    mode=1;
                    if (timeNow-startTime > 200) {
                        panHandler();
                    }
                    break;
                case MotionEvent.ACTION_POINTER_DOWN:
                    if (mode==2) break;
                    fingerDistance = getFingerDistance(event);
                    if ( fingerDistance > 5f) {
                        mode = 2;
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
            return  (int) Math.sqrt(dx2*dx2 + dy2*dy2);
        }
        //-------------------------------------------------------------------------
        private float getRotation(MotionEvent event) {
            // todo
            return  0;
        }
        //-------------------------------------------------------------------------
        private void downHandler() {
            AC_Log.print(" ########### down: " + startX + ", " + startY);
            String myEvent = "<TouchEvent type='down' x='" + startX + "' y='" + startY + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void upHandler() {
            AC_Log.print(" ########### up: " + startX + ", " + startY);
            String myEvent = "<TouchEvent type='up' x='" + startX + "' y='" + startY + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void singleTapHandler() {
            AC_Log.print(" ########### single tap: " + startX + ", " + startY);
            String myEvent = "<TouchEvent type='tap' x='" + startX + "' y='" + startY + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void doubleTapHandler() {
            AC_Log.print(" ########### double tap: " + startX + ", " + startY);
            String myEvent = "<TouchEvent type='doubleTap' x='" + startX + "' y='" + startY + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void longPressedHandler() {
            AC_Log.print(" ########### longPressed: " + startX + ", " + startY);
            String myEvent = "<TouchEvent type='longPressed' x='" + startX + "' y='" + startY + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void panHandler() {
            AC_Log.print(" ########### pan: " + startX + ", " + startY + " translate: " + dx + ", " + dy);
            String myEvent = "<GestureEvent type='pan' x='" + startX + "' y='" + startY + "' dx='" + dx + "' dy='"+ dy + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void pinchHandler() {
            AC_Log.print(" ########### pinch: factor: " + (float)fingerDistance/fingerDistanceStart);
            String myEvent = "<GestureEvent type='pinch' factor='" + (float)(fingerDistance)/fingerDistanceStart + "'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void rotationHandler() {
            //AC_Log.print(" ########### rotation: " + event.getX() + ", " + event.getY());
        }
        //-------------------------------------------------------------------------
        private void swipeLeftHandler() {
            AC_Log.print(" ########### swipe left");
            String myEvent = "<GestureEvent type='swipe-left' direction='left'/>";
            NativeBinding.onEvent(myEvent);
        }
        //-------------------------------------------------------------------------
        private void swipeRightHandler() {
            AC_Log.print(" ########### swipe right");
            String myEvent = "<GestureEvent type='swipe-right' direction='right'/>";
            NativeBinding.onEvent(myEvent);
        }
}
