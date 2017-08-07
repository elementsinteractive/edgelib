package nl.elements.edgelib;

import java.io.InputStream;
import java.io.IOException;
import java.io.FileInputStream;
import android.content.res.AssetManager;

class EdgelibFile
{
    String filename;
    InputStream filestream;
    AssetManager manager;
    int currentpos;
    int filesize;
    
    public EdgelibFile(String s, AssetManager am)
    {
	manager = am;
	fileopen(s);
	filename = s;
	filesize = -1;
    }
    
    protected void fileopen(String s)
    {
	reset();
	try 
	{
	    if(s.startsWith("::/"))
	    {
		// Asset
		filestream = manager.open(s.substring(3));
	    }
	    else
	    {
		// Regular file
		filestream = new FileInputStream(s);
	    }
	}
	catch (IOException e)
	{	    
	    reset();
	}
    }

    public void reset()
    {
	if (filestream != null) 
	{
	    try {
		filestream.close();
	    }
	    catch (IOException e) {}
	}
	filestream = null;
	currentpos = 0;
    }

    public static EdgelibFile open(AssetManager am, String s)
    {
	return new EdgelibFile(s, am);
    }
    
    public int isValid()
    {
	if (filestream != null) return 0;
	return -1;
    }

    public int read(int pointer, int bytes)
    {
	if (filestream == null) return -1;
	if (bytes < 0)
	{
	    debug("negative read: " + bytes);
	    return -3;
	}
	byte b[] = new byte[bytes];
	try
	{
	    int read = filestream.read(b, 0, bytes);
	    if (read > 0) 
	    {	
		sinkdata(b, pointer, read);
		currentpos += read;
	    }
	    return read;
	}
	catch (IOException e)
	{
	    reset();
	}
	return -2;
    }

    public int seekstart(int pos)
    {
	//debug("seek: " + pos);
	if (filestream == null) 
	    fileopen(filename);
	if (filestream == null) return -1;
	//debug("seek [1]");
	try
	{
	    if (pos > currentpos)
	    {
		//debug("seek [2]");
		int diff = pos - currentpos;
		int request = diff;
		while ((request > 0) && (diff > 0))
		{
		    //debug("seek [3]: " + request + ", " + diff);
		    request = (int) filestream.skip(diff);
		    //debug("seek [4]: " + request + ", " + diff);
		    diff -= request;
		    if (request > 0) currentpos += request;
		}
	    }
	    else if (pos < currentpos && pos >= 0)
	    {
		//debug("seek [5]");
		reset();
		//debug("seek [6]");
		fileopen(filename);
		//debug("seek [7]");
		return seekstart(pos);
	    }
	    //debug("seek [8]");
	    return currentpos;
	}
	catch (IOException e)
	{
	    //debug("seek [9]");
	    reset();
	    return -2;
	}
    }

    public int getLength()
    {
	if (filesize >= 0) return filesize;
	if (filestream == null) 
	    fileopen(filename);
	if (filestream == null) return -1;

	int restorepos = currentpos;
	
	int read = 0;
	int size = currentpos;
	try
	{
	    while ((read = (int) filestream.skip(65536)) > 0)
	    {
		size += read;
		currentpos += read;
	    }
	    filesize = size;
	}
	catch (IOException e)
	{
	    reset();
	    seekstart(restorepos);
	    return -1;
	}
	seekstart(restorepos);
	return filesize;
    }

    public int seekskip(int pos)
    {
	return seekstart(currentpos + pos);
    }

    public int seekend(int pos)
    {
	return seekstart(getLength() - pos);
    }

    private native void sinkdata(byte[] filedata, int pointer, int bytes);  
    private native void debug(String s);
}
