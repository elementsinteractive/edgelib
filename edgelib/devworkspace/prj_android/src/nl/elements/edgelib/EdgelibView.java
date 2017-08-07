package nl.elements.edgelib;

import android.view.View;
import android.view.MotionEvent;
import android.graphics.*;
import android.graphics.Canvas;
import android.content.Context;

class EdgelibView extends View implements EdgelibCommonView
{
    int[] imagedata;
    int orientation;
    int createdwidth;
    int createdheight;
    boolean firstframe;

    String message;

    float trackx;
    float tracky;

    public EdgelibView(Context c)
    {
        super(c);
        createdwidth = 0;
        createdheight = 0;
        imagedata = new int[createdwidth * createdheight];
        message = "";
        firstframe = true;

        trackx = 0;
        tracky = 0;

        this.setFocusable(true);
        this.setFocusableInTouchMode(true);
    }

    public void setOrientation(int neworientation)
    {
        orientation = neworientation;
        checkView();
    }

    public int getOrientation()
    {
        return orientation;
    }

    public void handleResume()
    {
        // nothing needed here
    }
    public void handlePause()
    {
        // nothing needed here
    }

    protected void checkView()
    {
        int w = getWidth();
        int h = getHeight();
        if (h != createdheight || w != createdwidth)
        {

            createdwidth = getWidth();
            createdheight = getHeight();
            imagedata = new int[createdwidth * createdheight];
            if (firstframe)
                onNewBuffer(imagedata, createdwidth, createdheight, orientation);
            else
                onChangeBuffer(imagedata, createdwidth, createdheight, orientation);

            firstframe = false;
        }
    }

    @Override
    protected void onDraw(Canvas c)
    {
        checkView();
        Paint p = new Paint();
        p.setColor(0xffffffff);

        onDrawEvent(imagedata, createdwidth, createdheight);

        c.drawBitmap(imagedata, 0, createdwidth,   
            0, 0, createdwidth, createdheight, 
            false, null);

        //String[] items = message.split("\n");
        //for (int i = 0; i < items.length; i++)
        //    c.drawText(items[i], 20, 120 + 10*i, p);
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

    public void setDebugText(String text)
    {
        message = text + "\n" + message;
    }

    protected native void onNewBuffer(int[] data, int width, int height, int orientation);
    protected native void onChangeBuffer(int[] data, int width, int height, int orientation);
    protected native void onDrawEvent(int[] data, int width, int height);

    protected native void trackEvent(int dx, int dy, int direction);

}
