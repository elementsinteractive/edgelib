package nl.elements.edgelib;

import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.view.MotionEvent;
import android.graphics.*;
import android.graphics.Canvas;
import android.content.Context;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

import android.util.Log;

class Edgelib3DView extends GLSurfaceView implements EdgelibCommonView
{
    int orientation;
    int createdwidth;
    int createdheight;

    Object lock;

    float trackx = 0, tracky = 0;
    boolean first = true;

    private Renderer glrenderer = new Renderer()
    {
        public void onDrawFrame(GL10 gl)
        {
            synchronized(lock)
            {
                //Log.d("EDGE", "frame");
                onGLFrame(gl);
            }
        }

        public void onSurfaceChanged(GL10 gl, int width, int height)
        {
            synchronized(lock)
            {
                createdwidth = width;
                createdheight = height;
                //Log.d("EDGE", "change");
                onGLDisplaySize(gl, width, height, orientation, first?1:0);
                first = false;
            }
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig cfg)
        {
            synchronized(lock)
            {
                //Log.d("EDGE", "create");

                if (first)
                    onGLInit(gl, cfg);
                else
                    onGLRestore(gl, cfg);
            }
        }
    };

    Edgelib3DView(Context c, Object lock)
    {
        super(c);
        this.lock = lock;
        this.setRenderer(glrenderer);
        this.setRenderMode(RENDERMODE_CONTINUOUSLY);

        this.setFocusable(true);
        this.setFocusableInTouchMode(true);
    }
     
    public void setOrientation(int neworientation)
    {
        orientation = neworientation;
    }

    public void handlePause()
    {
        this.onPause();
    }

    public void handleResume()
    {
        this.onResume();
    }

    public boolean onTrackballEvent(MotionEvent me)
    {
        trackx += me.getX();
        tracky += me.getY();
        while (trackx > 1)
        {
            trackEvent(1, 0, 0);
            trackx -= 1;
        }
        while (trackx < -1)
        {
            trackEvent(-1, 0, 0);
            trackx += 1;
        }
        while (tracky > 1)
        {
            trackEvent(0, 1, 0);
            tracky -= 1;
        }
        while (tracky < -1)
        {
            trackEvent(0, -1, 0);
            tracky += 1;
        }
        if (me.getAction() == MotionEvent.ACTION_DOWN) trackEvent(0,0,1);
        else if (me.getAction() == MotionEvent.ACTION_UP) trackEvent(0,0,-1);

        return true;
    }

    public int getOrientation()
    {
        return orientation;
    }

    public boolean checkInstallation()
    {
        return true;
    }

    protected native void trackEvent(int dx, int dy, int direction);

    protected native void onGLInit(Object gl, Object cfg);
    protected native void onGLRestore(Object gl, Object cfg);
    protected native void onGLDisplaySize(GL10 gl, int width, int height, int orientation, int first);
    protected native void onGLFrame(GL10 gl);

}
