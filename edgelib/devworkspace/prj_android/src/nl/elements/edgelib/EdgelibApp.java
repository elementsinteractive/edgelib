package nl.elements.edgelib;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.content.pm.ActivityInfo;
import android.view.Display;
import android.view.View;
import android.view.View.OnKeyListener;
import android.view.View.OnTouchListener;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager.LayoutParams;
import android.os.Build;
import android.os.Build.VERSION;
import android.os.Bundle;

import android.util.Log;

import java.io.File;

public class EdgelibApp extends Activity implements Runnable, OnKeyListener, OnTouchListener
{
    EdgelibCommonView mainView_2;
    View mainView;

    Thread eventThread;
    boolean suspend;
    boolean quit;
    boolean allownewframe;
    boolean fixorientation;
    boolean firstresume;
    AssetManager manager;
    EdgelibUtil utils;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        Context c = getBaseContext();

        if (!getOpenGL())
        {
            EdgelibView v = new EdgelibView(c);
            mainView = v;
            mainView_2 = v;
            //v.setDebugText("startup orient: " + getCurrentOrientation());
        } else {
            Edgelib3DView v = new Edgelib3DView(c, this);
            mainView = v;
            mainView_2 = v;
        }

        manager = getAssets();
        utils = new EdgelibUtil(c);

        allownewframe = true;
        suspend = true;
        fixorientation = false;
        firstresume = true;
        quit = false;
        new Thread(this).start();

        edgeStart(Build.MODEL, Build.PRODUCT, Integer.parseInt(Build.VERSION.SDK));
    }

    @Override
    public void onStop()
    {
        super.onStop();
        Log.d("EDGE", "Focus lost");
    }

    @Override
    public void onRestart()
    {
        super.onRestart();
        Log.d("EDGE", "Focus gained");
    }

    @Override
    public void onPause()
    {
        focusChanged(1);
        super.onPause();
        mainView_2.handlePause();
        Log.d("EDGE", "paused");
    }

    @Override
    public void onResume()
    {
        Log.d("EDGE", "resume");
        super.onResume();
        mainView_2.handleResume();

        if (firstresume)
            firstresume = false;
        else
            focusChanged(0);
        //mainView.requestFocus();
        
    }

    @Override
    public void onConfigurationChanged(Configuration config)
    {
        super.onConfigurationChanged(config);

        if (config.orientation == Configuration.ORIENTATION_PORTRAIT)
            mainView_2.setOrientation(0);
        else if (config.orientation == Configuration.ORIENTATION_LANDSCAPE)
            mainView_2.setOrientation(1);
        else
            mainView_2.setOrientation(0);
    }

    public int getCurrentOrientation()
    {
        if (mainView != null)
        {
            if (mainView.getWidth() > mainView.getHeight())
                return 1;
            else
                return 0;
        }

        Display getOrient = getWindowManager().getDefaultDisplay();

        int orientation = getOrient.getOrientation();

        if(orientation==Configuration.ORIENTATION_UNDEFINED)
        {
            Configuration config = getResources().getConfiguration();
            orientation = config.orientation;

            if(orientation==Configuration.ORIENTATION_UNDEFINED)
            {
                //if height and widht of screen are equal then
                // it is square orientation
                if(getOrient.getWidth()==getOrient.getHeight())
                {
                    orientation = Configuration.ORIENTATION_UNDEFINED;
                } else if (getOrient.getWidth() < getOrient.getHeight())
                {
                    orientation = Configuration.ORIENTATION_PORTRAIT;
                } else { 
                    orientation = Configuration.ORIENTATION_LANDSCAPE;
                }
            }
        }
        return orientation; // return value 1 is portrait and 2 is Landscape Mode
    }


    public int edgeWindowCreate(int options)
    {	
        //edgeDebug("options: " + options);

        if ((options & 1) == 1)
        {
            // fullscreen
            requestWindowFeature(Window.FEATURE_NO_TITLE); 
            getWindow().setFlags(LayoutParams.FLAG_FULLSCREEN, LayoutParams.FLAG_FULLSCREEN); 
        }
        if ((options & 2) == 2)
        {
            // force backlight
            getWindow().addFlags(LayoutParams.FLAG_KEEP_SCREEN_ON);
        }
        // forced orientations (AUTO and STARTUP are 0)
        if ((options & 12) == 4)
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        else if ((options & 12) == 8)
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        else if ((options & 12) == 12)
        {
            fixorientation = true;
        }

        setContentView(mainView);
        mainView.setOnKeyListener(this);
        mainView.setOnTouchListener(this);	
        mainView.requestFocus();

        return 0;
    }

    public void setBacklight(int enabled)
    {
        // thread safety, dispatch code to the correct thread.
        if (enabled > 0)
        {
            this.runOnUiThread(new Runnable(){
            public void run()
            {
                getWindow().addFlags(LayoutParams.FLAG_KEEP_SCREEN_ON);
            }
            });

        }
        else
        {
            this.runOnUiThread(new Runnable(){
            public void run()
            {
                getWindow().clearFlags(LayoutParams.FLAG_KEEP_SCREEN_ON);
            }
            });
        }
    }

    public void edgeDebug(String s)
    {	
        update();
        //((EdgelibView)mainView).setDebugText(s);
        //mainView.invalidate();
    }

    // android main loop
    public void run()
    {	

	    try {
            while (!quit)
            {
                //edgeDebug("1");

                while (getSuspend())
                {
                    //edgeDebug("2");

                    try
                    {
                        synchronized(this)
                        {
                            wait();
                        }
                    }
                    catch (InterruptedException ie)
                    {
                    }

                    //edgeDebug("3");

                    synchronized(this)
                    {
                        if (!getSustainLoop()) 
                        {
                            quit = true;
                            break;
                        }
                    }

                    //edgeDebug("4");
                }
                if (!getSustainLoop()) quit = true;

                //edgeDebug("5");

                if (getOpenGL())
                {
                    synchronized(this)
                    {
                        if (!quit) wait();
                    }
                } else if (!quit && allownewframe) 
                {	
                    allownewframe = false;
                    update();
                }

                //edgeDebug("6");
            }
        } catch (Exception e) 
        { 
            //edgeDebug("exception"); 
            //edgeDebug(e.getClass().getName()); 
            //return; 
        }

        finish();
    }

    public boolean onKey(View v, int code, KeyEvent ke)
    {
        //String s = "key: " + code;
        //edgeDebug(s);

        int down = 2;
        if (ke.getAction() == KeyEvent.ACTION_DOWN) down = 1;
        if (ke.getAction() == KeyEvent.ACTION_UP) down = -1;

        keyEvent(ke.getScanCode(), ke.getKeyCode(), down);

        return true;
    }
    
    public boolean onTouch(View v, MotionEvent me)
    {
        //String s = "move: (" + me.getX() + "," + me.getY() + ")";	
        //edgeDebug(s);
        
        int down = 0;
        if (me.getAction() == MotionEvent.ACTION_DOWN) down = 1;
        else if (me.getAction() == MotionEvent.ACTION_UP) down = -1;

        float x = me.getX();
        float y = me.getY();
        int index = 0;
        touchEvent(index, (int)x, (int)y, down);

        return true;
    }

    protected synchronized boolean getSuspend()
    {
        return suspend;
    }

    public void suspendloop(boolean newsuspend)
    {
        synchronized(this)
        {
            suspend = newsuspend;
            notify();
        }
    }
    
    public void quitloop()
    {
        synchronized(this)
        {
            quit = true;
            suspend = false;
            notify();
        }
    }

    public void firstframecheck()
    {
        if (fixorientation)
        {
            //edgeDebug("ff orientation: " + getCurrentOrientation());
            if (getCurrentOrientation() == 1)
            {
                setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
                mainView_2.setOrientation(1);
            }
            else
            {
                setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
                mainView_2.setOrientation(0);
            }
        }
    }

    public void update()
    {
        this.runOnUiThread(new Runnable(){
            public void run()
            {
                idleFrame();
                mainView.invalidate();
                allownewframe = true;
            }
        });
    }

    public AssetManager getAssetManager()
    {
        return manager;
    }

    public EdgelibUtil getUtility()
    {
        return utils;
    }

    public String getStoragePath()
    {
        File f = getFilesDir();
        return f.getAbsolutePath();
    }

    native void edgeStart(String customname, String devicename, int api);
    native void idleFrame();
    native boolean getSustainLoop();

    native void keyEvent(int keyscan, int keysym, int down);
    native void touchEvent(int index, int xpos, int ypos, int down);
    native void focusChanged(int focus);

    native boolean getOpenGL();
}
