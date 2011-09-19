package com.artcom.mobile.Base;

import java.util.HashMap;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class Sensors implements SensorEventListener{

    private static Sensors INSTANCE;
    private SensorManager _mySensorManager = null;
    private Context _myContext;

    public final static int ACCELEROMETER = Sensor.TYPE_ACCELEROMETER;
    public final static int GRAVITY = Sensor.TYPE_GRAVITY;
    public final static int GYROSCOPE = Sensor.TYPE_GYROSCOPE;
    public final static int LIGHT = Sensor.TYPE_LIGHT;
    public final static int LINEAR_ACCELERATION = Sensor.TYPE_LINEAR_ACCELERATION;
    public final static int MAGNETIC_FIELD = Sensor.TYPE_MAGNETIC_FIELD;
    public final static int ORIENTATION = Sensor.TYPE_ORIENTATION;
    public final static int PRESSURE = Sensor.TYPE_PRESSURE;
    public final static int PROXIMITY = Sensor.TYPE_PROXIMITY;
    public final static int ROTATION_VECTOR = Sensor.TYPE_ROTATION_VECTOR;
    public final static int TEMPERATURE = Sensor.TYPE_TEMPERATURE;

    private static final HashMap<Integer, String> eventNameForType = new HashMap<Integer, String>() {
        private static final long serialVersionUID = 1L; {
        put(Sensor.TYPE_ACCELEROMETER,  "ACCELEROMETER");
        put(Sensor.TYPE_GRAVITY,        "GRAVITY");
        put(Sensor.TYPE_GYROSCOPE,      "GYROSCOPE");
        put(Sensor.TYPE_LIGHT,          "LIGHT");
        put(Sensor.TYPE_LINEAR_ACCELERATION, "LINEAR_ACCELERATION");
        put(Sensor.TYPE_MAGNETIC_FIELD, "MAGNETIC_FIELD");
        put(Sensor.TYPE_ORIENTATION,    "ORIENTATION");
        put(Sensor.TYPE_PRESSURE,       "PRESSURE");
        put(Sensor.TYPE_PROXIMITY,      "PROXIMITY");
        put(Sensor.TYPE_ROTATION_VECTOR,"ROTATION_VECTOR");
        put(Sensor.TYPE_TEMPERATURE,    "TEMPERATURE");
    }};
    //------CONSTRUCTOR--------------------------------------------------------
    public Sensors(Context context) {
        if(INSTANCE!=null)INSTANCE.disableAll();
        _myContext = context;
        _mySensorManager = (SensorManager) _myContext.getSystemService(Context.SENSOR_SERVICE);
        INSTANCE = this;
    }
    //-------STATIC------------------------------------------------------------
    public static void setContext(Context context) {
        disableAllSensors();
        INSTANCE = new Sensors(context);
    }
    public static void enableSensor(int sensorType) {
        if (INSTANCE != null) INSTANCE.enable(sensorType);
        else AC_Log.print("Sensors: Error - no context given, call Constructor or setContext()");
    }
    //-------------------------------------------------------------------------
    public static void disableSensor(int sensorType) {
        if (INSTANCE != null) INSTANCE.disable(sensorType);
    }
    //-------------------------------------------------------------------------
    public static void disableAllSensors() {if (INSTANCE != null) INSTANCE.disableAll();}
    //-------MEMBER------------------------------------------------------------
    public void enable(int sensorType) {
        _mySensorManager.registerListener(this,
                _mySensorManager.getDefaultSensor(sensorType),
                SensorManager.SENSOR_DELAY_UI);
    }
    //-------------------------------------------------------------------------
    public void disable(int sensorType) {
        _mySensorManager.unregisterListener(this, _mySensorManager.getDefaultSensor(sensorType));
    }
    //-------------------------------------------------------------------------
    public void disableAll() {
        _mySensorManager.unregisterListener(this);
    }
    //-------------------------------------------------------------------------
    public void onAccuracyChanged(Sensor sensor, int accuracy) { }
    //-------------------------------------------------------------------------
    public void onSensorChanged(SensorEvent event) {
        StringBuilder sb = new StringBuilder("<SensorEvent type='");
        sb.append(eventNameForType.get(event.sensor.getType())).append("'");
        for(int i=0, l = event.values.length; i<l; ++i) {
            sb.append(" value").append(i).append("='").append(event.values[i]).append("'");
        }
        sb.append("/>");
        //AC_Log.print(sb.toString());
        NativeBinding.onEvent(sb.toString());
    }
}
