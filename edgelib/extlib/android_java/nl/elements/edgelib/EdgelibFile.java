package nl.elements.edgelib;

import java.io.InputStream;
import java.io.IOException;
import java.io.FileInputStream;
import java.io.File;
import android.content.res.AssetManager;
import android.util.Log;

class EdgelibFile
{
    String filename;
    InputStream filestream;
    AssetManager manager;
    int currentpos;
    int filesize;
    String[] filelist;
    
    public EdgelibFile(String s, AssetManager am)
    {
        manager = am;
        fileopen(s);
        filename = s;
        filesize = -1;
    }

    public EdgelibFile(AssetManager am)
    {
        manager = am;
        reset();
        filename = "";
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
            Log.d("EDGE", "Opening " + s + " failed");
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

    public static EdgelibFile opendir(AssetManager am, String s)
    {
        EdgelibFile f = new EdgelibFile(am);
        f.readdir(s);
        return f;
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
            Log.d("EDGE", "negative read: " + bytes);
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

    public int readdir(String directory)
    {
        filelist = null;
        try
        {
            Log.d("EDGE", directory);
            if(directory.startsWith("::/"))
            {
                while ((directory.length() > 3) && directory.endsWith("/"))
                {
                    directory = directory.substring(0, directory.length() - 1);
                    Log.d("EDGE", "" + directory + ", " + directory.length());
                }
                
                Log.d("EDGE:JFile", "Opening: " + directory.substring(3));
                filelist = manager.list(directory.substring(3));
                for (int i = 0; i < filelist.length; i++)
                {
                    Log.d("EDGE:JFile", filelist[i]);
                    //filelist[i] = directory + filelist[i];
                }
            }
            else
            {
                File folder = new File(directory);
                File[] syslist = folder.listFiles();
                filelist = new String[syslist.length];
                for (int i = 0; i < filelist.length; i++)
                {
                    Log.d("EDGE:CFile", syslist[i].getName());
                    filelist[i] = syslist[i].getName();
                }
            }
        }
        catch (IOException e)
        {
            Log.d("EDGE", "Unable to open directory " + directory);
            filelist = null;
        }
        
        return getDirectoryCount();
    }

    public int getDirectoryCount()
    {
        if (filelist == null)
            return -1;
        return filelist.length;
    }

    public String getDirectoryEntry(int index)
    {
        if (filelist == null) return null;
        if (index < 0 || index >= filelist.length) return null;
        return filelist[index];
    }

    private native void sinkdata(byte[] filedata, int pointer, int bytes);
}
