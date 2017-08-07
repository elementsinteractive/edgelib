package nl.elements.edgelib;

import android.content.Context;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;

import android.util.Log;

import android.os.Vibrator;
import android.os.BatteryManager;

import android.telephony.TelephonyManager;

import java.util.Locale;
import java.util.TimeZone;

import java.lang.reflect.Field;

class EdgelibUtil 
{  
    private int batterylevel = 0;
    private int batterystate = 0;
    private boolean batterylive = false;

    public Context context;
    Vibrator vibrator = null;

    public BroadcastReceiver batteryreceiver = new BroadcastReceiver() 
    {
	public void onReceive(Context c, Intent intent)
	{
	    int newlevel = intent.getIntExtra("level", -1);
            int scale = intent.getIntExtra("scale", 100);
	    if (newlevel >= 0)
	    {
		batterylevel = newlevel * 255 / scale;
		batterylive = true;
		int status = intent.getIntExtra("status", BatteryManager.BATTERY_STATUS_UNKNOWN);
                
                if (status == BatteryManager.BATTERY_STATUS_CHARGING) 
		{
		    batterystate = 3; // BATT_CHARGING
                } else if (status == BatteryManager.BATTERY_STATUS_DISCHARGING) {
                    batterystate = 4; // BATT_BATTERY_POWERED
                } else if (status == BatteryManager.BATTERY_STATUS_NOT_CHARGING) {
                    batterystate = 5; // BATT_AC_POWERED
                } else if (status == BatteryManager.BATTERY_STATUS_FULL) {
                    batterystate = 5; // BATT_AC_POWERED
                } else {
                    batterystate = 0;
                }

	    }
	}
    };

    public EdgelibUtil(Context c)
    {
	context = c;
	Intent i = c.registerReceiver(batteryreceiver, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
	if (i != null) batteryreceiver.onReceive(c, i);
    }

    public void vibrate()
    {
	try {
	    if (vibrator == null) vibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
	    vibrator.vibrate(500);
	} catch (Exception e) {
	    // Permission was not granted
	    return;
	}
    }

    public boolean getBatteryValid()
    {
	return batterylive;
    }

    public int getBatteryLevel()
    {
	if (!getBatteryValid()) return 255;
	return batterylevel;
    }

    public int getBatteryState()
    {
	return batterystate;
    }

    public String getIMEI()
    {
	try {
	    TelephonyManager telephonyManager = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
	    String imei = telephonyManager.getDeviceId();
	    if (imei == null) return "";
	    return imei;
	}
	catch (Exception e)
	{
	    // Permission was not granted
	    return "";
	}
    }

    public String getSerial()
    {
	Class c = null;
	try {
	    c = Class.forName("android.os.Build");
	} catch (ClassNotFoundException ignored) {}
	if (c == null) return "";
	Field f = null;
	try {
	    f = c.getField("SERIAL");
	} catch (Throwable ignored) {}
	if (f == null) return "";
	String o = "";
	try {
	    o = (String) f.get(null); // static field
	} catch (Throwable ignored) {}
	return o;
    }

    public String getLocale()
    {
	Locale l = Locale.getDefault();
	String langpart = l.getLanguage().toLowerCase(Locale.US);
	String varpart = l.getCountry().toLowerCase(Locale.US);
	if (varpart.equals(""))
	    return langpart;
	return(langpart + "-" + varpart);
    }

    public int getTimezoneOffset()
    {
	return (TimeZone.getDefault().getOffset(System.currentTimeMillis()) / 1000);
    }
}