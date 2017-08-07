package nl.elements.edgelib.eventcore;

import android.app.Activity;
import android.os.Bundle;
import nl.elements.edgelib.EdgelibApp;

public class Eventcore extends EdgelibApp
{
    static
    {
	System.loadLibrary("eventcore");
    }
}
