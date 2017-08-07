// linuxcam.h
// Linux camera class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////


#include <fcntl.h> // open(), close(), read(), write()
#include <dirent.h> // opendir(), closedir(), readdir()
#include <sys/ioctl.h> // ioctl()
#include <sys/mman.h> // mmap(), munmap()
#include <linux/videodev2.h> // v4l2_*, VIDIOC_*
#include <string.h> // strncmp()
#include <stdio.h> // sprintf()

/* Note that linux/videodev* is BSD licensed, which means that the 
 * relevant copyright and disclaimer should be mentioned within
 * the documentation (in this case, EDGEs)
 */

typedef struct _mmaprange
{
    void * offset;
    size_t length;
} mmaprange;


typedef struct _resolution
{
    int width;
    int height;
    int pitch;
    int format_v4l;
    int format_edge;
    int bpp;
} resolution;

class V4LCamera
{

    public:
        V4LCamera(ClassECamera *, ClassEdge *);
        ~V4LCamera();

	static int getCamera(int);
	ERESULT init(int);
	ERESULT startCapture(void);
	ERESULT pauseCapture(void);

	int getDefaultSettingCount();
	void getDefaultSetting(int, E_CAMERAMODE &);
	void getCurrentSetting(E_CAMERAMODE &);

	ERESULT setResolution(unsigned int, unsigned int, unsigned int);

	void poll(void);

	int getState() { return status; }
	int getIndex() { return index; }

    protected:
	void cleanup(void);
	int probeformat(resolution * base, int fillcount, int edgeformat, unsigned int v4lformat, bool * method);
	int genfixed(resolution * base, unsigned int v4lformat, int minx, int miny, int maxx, int maxy, int * remaining);
	int genpredef(resolution * base, int fillcount, unsigned int v4lformat);
	void buildresolutiontable(void);
	ERESULT pipeCreate(v4l2_format *);
	void pipeDelete();

        int driverfn; // the file handle of /dev/video*

        v4l2_capability vcap;
        v4l2_requestbuffers bufferconfig;

	resolution * settings;
	int settingcount;

        v4l2_buffer * buffers; // the V4L2 buffer handles
	mmaprange * mmapbuffers; // the buffers used for memory-mapped data transfer
        int buffercount;

	ClassECamera * frontend;
	ClassEdge * framework;
	int status;
	int index;
	int deviceindex;

	resolution currentmode;

#define usedbuffers 3

};

const static unsigned int v4lformats[10]=
                       {V4L2_PIX_FMT_RGB555,
			V4L2_PIX_FMT_RGB565,
			V4L2_PIX_FMT_RGB24,
			V4L2_PIX_FMT_BGR24,
			V4L2_PIX_FMT_RGB32,
			V4L2_PIX_FMT_BGR32,
			V4L2_PIX_FMT_YUYV,
			V4L2_PIX_FMT_UYVY,
			V4L2_PIX_FMT_YUV420,
			V4L2_PIX_FMT_YVU420}; // 10 options
const static int dspformats[10]=
		       {EDSP_TRUE15,
			EDSP_TRUE16,
			EDSP_TRUE24_INV,
			EDSP_TRUE24,
			EDSP_TRUE32_INV,
			EDSP_TRUE32,
			EDSP_YUYV,
			EDSP_UYVY,
			EDSP_YUV12,
			EDSP_YVU12};

static void ecCamRegister(ECOREOBJ);
static void ecCamUnregister(ECOREOBJ);

//
// Camera implementation
//

V4LCamera::V4LCamera(ClassECamera * myfrontend, ClassEdge * mybackend)
{
    this->frontend = myfrontend;
    this->framework = mybackend;
    this->status = ECS_INVALID;
    this->index = -1;
    this->driverfn = -1;
    this->buffers = NULL;
    this->mmapbuffers = NULL;
    this->buffercount = 0;
}

V4LCamera::~V4LCamera()
{
    cleanup();
}

void V4LCamera::cleanup()
{
    status = ECS_INVALID;
    pipeDelete();
    ecCamUnregister(this);
    if (status && (driverfn > 0))
    {
 	ioctl(driverfn, VIDIOC_STREAMOFF, &bufferconfig);
    }
    if (settings)
    {
        delete settings;
        settings = NULL;
        settingcount = 0;
    }
    if (driverfn)
    {
        close(driverfn);
    }
    driverfn = -1;
}


int V4LCamera::getCamera(int index)
{
    DIR * devdir = opendir("/dev");
    if (!devdir) 
	return (index == -1) ? 0 : -1;

    int count = 0;

    while (dirent * entry = readdir(devdir))
    {
	if (strncmp(entry->d_name, "video", 5) == 0)
	{            
	    if (count == index)
            {
                int vno = atoi(&(entry->d_name[5]));
                closedir(devdir);
                return vno;
            }
	    count++;
        }
    }

    closedir(devdir);

    if (index == -1) return count;
    return -1;
}

ERESULT V4LCamera::init(int index)
{
    if (driverfn > 0)
    {
        cleanup(); // shut down the device we already have
    }

    if (index < 0)
    {
        if (index == ELOC_REAR) 
            if(init(0) == E_OK) return E_OK;
        if (index == ELOC_FRONT) 
            if(init(1) == E_OK) return E_OK;

        int lastcam = getCamera(-1);
        for (int i = 0; i < lastcam; i++)
	    if(init(i) == E_OK) return E_OK;
        return E_NOTEXISTS;
    }
    else 
    {        
	this->index = index;

    	int devdrv = getCamera(index);
	if(devdrv < 0) return E_NOTEXISTS;
	char devname[128];
	sprintf(devname, "/dev/video%i", devdrv);
        
        // open the device
        driverfn = open(devname, O_RDWR | O_NONBLOCK);
        if (driverfn <= 0) return E_NOTEXISTS;
	deviceindex = devdrv;
        
        // check if it is a Video4Linux 2 device
        if (ioctl(driverfn, VIDIOC_QUERYCAP, &vcap) < 0)
            return E_UNSUPPORTED;
            
        // make sure it is an input device
	if ((vcap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) return E_UNSUPPORTED; 

        // make sure it can do Streaming I/O
	if ((vcap.capabilities & V4L2_CAP_STREAMING) == 0) return E_UNSUPPORTED;

	// enumerate formats and resolutions
	buildresolutiontable();
	if (settingcount == 0) return E_ERROR;

	// set default mode
	ERESULT retval = setResolution(settings[0].width, settings[0].height, settings[0].format_v4l);
	if (retval != E_OK) return retval;
	
    }

    return E_OK;
}


void V4LCamera::buildresolutiontable(void)
{

    int counts[10];
    bool methods[10];
    int totalcount = 0;
    for (int i = 0; i < 10; i++)
    {
	methods[i] = false;
	counts[i] = probeformat(NULL, 0, dspformats[i], v4lformats[i], &(methods[i]));
        totalcount += counts[i];
	//ClassEdge::Log("mode ", false); ClassEdge::Log(i, false);
	//ClassEdge::Log(" returned ", false); ClassEdge::Log(counts[i], false);
	//ClassEdge::Log(" method ", false); ClassEdge::Log((int)methods[i]);
    }

    settings = new resolution[totalcount];
    int basecount = 0;
    for (int i = 0; i < 10; i++)
    {
	basecount += probeformat(&(settings[basecount]), counts[i], dspformats[i], v4lformats[i], &(methods[i]));
    }
    settingcount = basecount;
 

}

int V4LCamera::probeformat(resolution * base, int fillcount, int edgeformat, unsigned int v4lformat, bool * method)
{
    v4l2_format vspec;
    memset(&vspec, 0, sizeof(v4l2_format));
    vspec.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(driverfn, VIDIOC_G_FMT, &vspec) < 0) return 0;    
    //ClassEdge::Log("G... ", false);
    vspec.fmt.pix.pixelformat = v4lformat;
    if (ioctl(driverfn, VIDIOC_TRY_FMT, &vspec) < 0) return 0;
    //ClassEdge::Log("T... ", false);
    if (vspec.fmt.pix.pixelformat != v4lformat) return 0; // this format is not supported

    //ClassEdge::Log("mode ", false); ClassEdge::Log(v4lformat, false); ClassEdge::Log("supported");

    if (base == NULL)
    {
        int ticker = 16;
	*method = false;
        int count = genfixed(NULL, v4lformat, 1, 1, 99999, 99999, &ticker);
	//ClassEdge::Log("fixed probe for ", false); ClassEdge::Log(v4lformat, false);
	//ClassEdge::Log(" returned ", false); ClassEdge::Log(count);
	if(ticker == 0)
	{
	    //this camera has built-in scaling support. go back and fill up with meaningful resolutions
	    count = genpredef(NULL, 0, v4lformat);
	    //ClassEdge::Log("dynamic probe for ", false); ClassEdge::Log(v4lformat, false);
	    //ClassEdge::Log(" returned ", false); ClassEdge::Log(count);
	    *method = true;
	}
        return count;
    }
    else
    {
	int count = 0;
	if(*method == true)
        {
            count = genpredef(base, fillcount, v4lformat);
        }
	else
	{
	    int ticker = fillcount;
            genfixed(base, v4lformat, 1, 1, 99999, 99999, &ticker);
	    while (ticker > 0)
	    {
		ticker--;
		base[ticker] = base[fillcount - ticker - 1];
	    }
        }
	
	for (int i = 0; i < count; i++)
	{
	    base[i].format_edge = edgeformat;
	}
	return count;
    }
}

int V4LCamera::genfixed(resolution * base, unsigned int v4lformat, int minx, int miny, int maxx, int maxy, int * remaining)
{

    if (maxx - minx < 0) return 0;
    if (maxy - miny < 0) return 0;
    if (*remaining == 0) return 0;

    v4l2_format vspec;
    memset(&vspec, 0, sizeof(v4l2_format));
    vspec.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(driverfn, VIDIOC_G_FMT, &vspec) < 0) return 0;    
    vspec.fmt.pix.pixelformat = v4lformat;
    int selx = (minx + maxx) >> 1;
    int sely = (miny + maxy) >> 1;
    vspec.fmt.pix.width = selx;
    vspec.fmt.pix.height = sely;
    if (ioctl(driverfn, VIDIOC_TRY_FMT, &vspec) < 0) return 0;
    int newx = vspec.fmt.pix.width;
    int newy = vspec.fmt.pix.height;
    
//    ClassEdge::Log(newx, false); ClassEdge::Log(" x ", false); ClassEdge::Log(newy);

    if (newx < minx && newy < miny)
	return genfixed(base, v4lformat, selx + 1, sely + 1, maxx, maxy, remaining);
    else if (newx < minx)
	return genfixed(base, v4lformat, selx + 1, miny, maxx, maxy, remaining);
    else if (newy < miny)
	return genfixed(base, v4lformat, minx, sely + 1, maxx, maxy, remaining);
    else if (newx > maxx && newy > maxy)
	return genfixed(base, v4lformat, minx, miny, selx - 1, sely - 1, remaining);
    else if (newx > maxx)
	return genfixed(base, v4lformat, minx, miny, selx - 1, maxy, remaining);
    else if (newy > maxy)
	return genfixed(base, v4lformat, minx, miny, maxx, sely - 1, remaining);
    
    (*remaining)--;
    if (base)
    {
        base[*remaining].width = newx;
        base[*remaining].height = newy;
        base[*remaining].format_v4l = v4lformat;
        base[*remaining].pitch = vspec.fmt.pix.bytesperline;
    }
    if (remaining == 0) return 1;
    return
        ( genfixed(base, v4lformat, minx, miny, newx - 1, newy - 1, remaining)
    	+ genfixed(base, v4lformat, newx+1, newy+1, maxx, maxy, remaining)
        + 1);
}

int V4LCamera::genpredef(resolution * base, int fillcount, unsigned int v4lformat)
{
    v4l2_format vspec;
    memset(&vspec, 0, sizeof(v4l2_format));
    vspec.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(driverfn, VIDIOC_G_FMT, &vspec) < 0) return 0;    
    vspec.fmt.pix.pixelformat = v4lformat;
    vspec.fmt.pix.width = 99999;
    vspec.fmt.pix.height = 99999;
    if (ioctl(driverfn, VIDIOC_TRY_FMT, &vspec) < 0) return 0;
    unsigned int xmax = vspec.fmt.pix.width;
    //unsigned int ymax = vspec.fmt.pix.height;
    vspec.fmt.pix.width = 1;
    vspec.fmt.pix.height = 1;
    if (ioctl(driverfn, VIDIOC_TRY_FMT, &vspec) < 0) return 0;
    //unsigned int xmin = vspec.fmt.pix.width;
    //unsigned int ymin = vspec.fmt.pix.height;

    //ClassEdge::Log("running predef loop until ", false); ClassEdge::Log(xmax);

    int count = 0;

    // 4:3, group 1 (VGA)
    for (unsigned int xc = 80; xc <= xmax; xc *= 2)
    {
	vspec.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	vspec.fmt.pix.width = xc;
    	vspec.fmt.pix.height = xc * 3 / 4;
        vspec.fmt.pix.pixelformat = v4lformat;
    	if (ioctl(driverfn, VIDIOC_TRY_FMT, &vspec) < 0) return 0;
        if (vspec.fmt.pix.width == xc && vspec.fmt.pix.height == (xc * 3 / 4) && vspec.fmt.pix.pixelformat == v4lformat)
        {
            count++;
	    if(base)
	    {
	        base[count-1].width = xc;
	        base[count-1].height = xc*3/4;
	        base[count-1].format_v4l = v4lformat;
	        base[count-1].pitch = vspec.fmt.pix.bytesperline;
	    }
            // push resolution
        } 
	if (base && count == fillcount) return count;
    }

    //ClassEdge::Log("4:3 ", false);

    // 3:2, group 1 (HVGA)
    for (unsigned int xc = 60; xc <= xmax; xc *= 2)
    {
	vspec.fmt.pix.width = xc;
    	vspec.fmt.pix.height = xc * 2 / 3;
    	if (ioctl(driverfn, VIDIOC_TRY_FMT, &vspec) < 0) return 0;
        if (vspec.fmt.pix.width == xc && vspec.fmt.pix.height == (xc * 2 / 3) && vspec.fmt.pix.pixelformat == v4lformat)
        {
            count++;
	    if(base)
	    {
	        base[count-1].width = xc;
	        base[count-1].height = xc*2/3;
	        base[count-1].format_v4l = v4lformat;
	        base[count-1].pitch = vspec.fmt.pix.bytesperline;
	    }
        } 
	if (base && count == fillcount) return count;
    }

    //ClassEdge::Log("3:2 ", false);

    // 11:9, group 1 (common webcam format, DVD)
    for (unsigned int xc = 88; xc <= xmax; xc *= 2)
    {
	vspec.fmt.pix.width = xc;
    	vspec.fmt.pix.height = xc * 9 / 11;
    	if (ioctl(driverfn, VIDIOC_TRY_FMT, &vspec) < 0) return 0;
        if (vspec.fmt.pix.width == xc && vspec.fmt.pix.height == (xc * 9 / 11) && vspec.fmt.pix.pixelformat == v4lformat)
        {
            count++;
	    if(base)
	    {
	        base[count-1].width = xc;
	        base[count-1].height = xc*9/11;
	        base[count-1].format_v4l = v4lformat;
	        base[count-1].pitch = vspec.fmt.pix.bytesperline;
	    }
        } 
	if (base && count == fillcount) return count;
    }

    //ClassEdge::Log("11:9 ", false);

    // 16:9, group 1 (widescreen)
    for (unsigned int xc = 240; xc <= xmax; xc *= 2)
    {
	vspec.fmt.pix.width = xc;
    	vspec.fmt.pix.height = xc * 9 / 16;
    	if (ioctl(driverfn, VIDIOC_TRY_FMT, &vspec) < 0) return 0;
        if (vspec.fmt.pix.width == xc && vspec.fmt.pix.height == (xc * 9 / 16) && vspec.fmt.pix.pixelformat == v4lformat)
        {
            count++;
	    if(base)
	    {
	        base[count-1].width = xc;
	        base[count-1].height = xc*9/16;
	        base[count-1].format_v4l = v4lformat;
	        base[count-1].pitch = vspec.fmt.pix.bytesperline;
	    }
        } 
	if (base && count == fillcount) return count;
    }

    //ClassEdge::Log("16:9 ", false);

    // 5:3, group 1 (Super 16 film)
    for (unsigned int xc = 50; xc <= xmax; xc *= 2)
    {
	vspec.fmt.pix.width = xc;
    	vspec.fmt.pix.height = xc * 3 / 5;
    	if (ioctl(driverfn, VIDIOC_TRY_FMT, &vspec) < 0) return 0;
        if (vspec.fmt.pix.width == xc && vspec.fmt.pix.height == (xc * 3 / 5) && vspec.fmt.pix.pixelformat == v4lformat)
        {
            count++;
	    if(base)
	    {
	        base[count-1].width = xc;
	        base[count-1].height = xc*3/5;
	        base[count-1].format_v4l = v4lformat;
	        base[count-1].pitch = vspec.fmt.pix.bytesperline;
	    }
        } 
	if (base && count == fillcount) return count;
    }

    //ClassEdge::Log("5:3 and finished with: ", false);
    //ClassEdge::Log(count);

    return count;
}

int V4LCamera::getDefaultSettingCount()
{
    return settingcount;
}
void V4LCamera::getDefaultSetting(int index, E_CAMERAMODE & mode)
{
    if (index < 0) return;
    if (index >= settingcount) return;

    mode.x = settings[index].width;
    mode.y = settings[index].height;
    mode.format = settings[index].format_edge;
    mode.expectedfps = 0;

    //int format_v4l;
    //int format_edge;
    //int bpp;
}

void V4LCamera::getCurrentSetting(E_CAMERAMODE & mode)
{
    if (status == ECS_INVALID)
    {
        mode.x = 0;
        mode.y = 0;
        mode.format = 0;
        mode.expectedfps = 0;
    }
    else
    {
        mode.x = currentmode.width;
        mode.y = currentmode.height;
        mode.format = currentmode.format_edge;
        mode.expectedfps = 0;
    }
}

ERESULT V4LCamera::setResolution(unsigned int x, unsigned int y, unsigned int v4lformat)
{
    pauseCapture();
    status = ECS_INVALID;

    v4l2_format vspec;
    memset(&vspec, 0, sizeof(v4l2_format));
    vspec.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(driverfn, VIDIOC_G_FMT, &vspec) < 0) return E_ERROR;
    vspec.fmt.pix.pixelformat = v4lformat;
    vspec.fmt.pix.width = x;
    vspec.fmt.pix.height = y;
    if (ioctl(driverfn, VIDIOC_TRY_FMT, &vspec) < 0) return E_ERROR;
    if (vspec.fmt.pix.width == x && vspec.fmt.pix.height == y && vspec.fmt.pix.pixelformat == v4lformat) // test if supported
    {

	pipeDelete();

	// try to change stream directly
        if (ioctl(driverfn, VIDIOC_S_FMT, &vspec) < 0)
        {                  
            char devname[256];
	    sprintf(devname, "/dev/video%i", deviceindex);
	    close(driverfn); // we have to reset the camera
	    driverfn = open(devname, O_RDWR | O_NONBLOCK);
	    if (driverfn <= 0) 
	    {
	        framework->OnCameraChange(frontend, E_ABORTED);
		return E_ABORTED;
	    }

	    if (ioctl(driverfn, VIDIOC_S_FMT, &vspec) < 0) 
	    {
	        framework->OnCameraChange(frontend, E_ERROR);
		return E_ERROR;
	    }
	}
	// todo: setup pipeline
	if (pipeCreate(&vspec) != E_OK)
	{
            framework->OnCameraChange(frontend, E_ERROR);
            return E_ERROR;
	}

	currentmode.width = x;
	currentmode.height = y;
	currentmode.format_v4l = v4lformat;
	currentmode.format_edge = EDSP_NONE;
	for (int i = 0; i < 10; i++) if (v4lformats[i] == v4lformat) currentmode.format_edge = dspformats[i];
		
	status = ECS_READY;

        framework->OnCameraChange(frontend, E_OK);
	return E_OK;
    }
    else return E_UNSUPPORTED;
}

ERESULT V4LCamera::pipeCreate(v4l2_format * selectedformat)
{
    v4l2_requestbuffers vreqbuf;
    memset(&vreqbuf, 0, sizeof(v4l2_requestbuffers));
    vreqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vreqbuf.memory = V4L2_MEMORY_MMAP;
    vreqbuf.count = usedbuffers; // you may want to have this configurable...

    //ClassEdge::Log("pipe 1");

    if (ioctl (driverfn, VIDIOC_REQBUFS, &vreqbuf) >= 0)
    {	
	buffers = new v4l2_buffer[vreqbuf.count];
	mmapbuffers = new mmaprange[vreqbuf.count];

	for (unsigned int i = 0; i < vreqbuf.count; i++)
	{
	    // to prevent errors after bailouts
	    mmapbuffers[i].length = 0;
	    mmapbuffers[i].offset = NULL;
	}
	buffercount = vreqbuf.count;

        //ClassEdge::Log("pipe 2");

	for (unsigned int i = 0; i < vreqbuf.count; i++)
	{
	    // describe buffer
            v4l2_buffer vbuf;
            memset(&vbuf, 0, sizeof(v4l2_buffer));
            vbuf.type = vreqbuf.type;
            vbuf.memory = V4L2_MEMORY_MMAP;
            vbuf.index = i;

            // mmap it
            if (ioctl(driverfn, VIDIOC_QUERYBUF, &vbuf) < 0) return E_ERROR;
            mmapbuffers[i].length = vbuf.length;
            mmapbuffers[i].offset = mmap(NULL, vbuf.length, PROT_READ | PROT_WRITE, MAP_SHARED, driverfn, vbuf.m.offset);           
            buffers[i] = vbuf;
 
            // enqueue buffers just-in-case
            memset(&vbuf, 0, sizeof(v4l2_buffer));
            vbuf.type = vreqbuf.type;
            vbuf.memory = V4L2_MEMORY_MMAP;
            vbuf.index = i;
            ioctl(driverfn, VIDIOC_QBUF, &vbuf);
	}

        //ClassEdge::Log("pipe 3");

	v4l2_buf_type vtype;
	vtype = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(driverfn, VIDIOC_STREAMON, &vtype) < 0) return E_ERROR;

        //ClassEdge::Log("pipe 4");

	return E_OK;
    }
    else return E_UNSUPPORTED;
}

void V4LCamera::pipeDelete()
{
    if (buffercount > 0 && buffers && mmapbuffers)
    {
        // unmap buffers
        for (int i = 0; i < buffercount; i++)
        {
            if (mmapbuffers[i].length > 0)
                munmap(mmapbuffers[i].offset, mmapbuffers[i].length);
        }
	delete mmapbuffers;
	delete buffers;
	mmapbuffers = NULL;
	buffers = NULL;
	buffercount = 0;
    }
}

ERESULT V4LCamera::startCapture()
{
    if (status == ECS_INVALID) return E_ERROR;
    ecCamRegister(this);
    status = ECS_RECORDING;
    return E_OK;
}

ERESULT V4LCamera::pauseCapture()
{
    if (status == ECS_INVALID) return E_ERROR;
    ecCamUnregister(this);
    status = ECS_READY;
    return E_OK;
}

void V4LCamera::poll()
{
    v4l2_buffer vbuf;
    memset(&vbuf, 0, sizeof(v4l2_buffer));
    vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    vbuf.memory = V4L2_MEMORY_MMAP;

    while (ioctl(driverfn, VIDIOC_DQBUF, &vbuf) >= 0)
    {
	//ClassEdge::Log("frame");

	E_CAMERAFRAME frame;
	frame.data = (unsigned char *) mmapbuffers[vbuf.index].offset;
	frame.width = currentmode.width;
	frame.height = currentmode.height;
	frame.format = currentmode.format_edge;
	frame.pitch = currentmode.pitch; // fixme
	frame.orientation = 0;
	framework->OnCameraFrame(frontend, &frame);

	ioctl(driverfn, VIDIOC_QBUF, &vbuf);	
    }
}

//
// Camera callback functions
//

int ecCamDeviceCount(void)
{
    return V4LCamera::getCamera(-1);
}

ECOREOBJ ecCamCreate(ECOREOBJ framework, ECOREOBJ frontend)
{
    return new V4LCamera((ClassECamera*) frontend, (ClassEdge*) framework);
}

void ecCamDelete(ECOREOBJ camera)
{
    if (camera) delete ((V4LCamera *)camera);
}

unsigned long ecCamRequestCamera(ECOREOBJ camera, int index)
{
    return ((V4LCamera *)camera)->init(index);
}

int ecCamGetIndex(ECOREOBJ camera)
{
    return ((V4LCamera *)camera)->getIndex();
}

int ecCamGetOrientation(ECOREOBJ camera)
{
    return ELOC_UNKNOWN;
}

unsigned long ecCamStartCapture(ECOREOBJ camera)
{
    return ((V4LCamera *)camera)->startCapture();
}

unsigned long ecCamStopCapture(ECOREOBJ camera)
{
    return ((V4LCamera *)camera)->pauseCapture();
}

int ecCamGetState(ECOREOBJ camera)
{
    return ((V4LCamera *)camera)->getState();
}

int ecCamGetResolutions(ECOREOBJ camera)
{
    return ((V4LCamera *)camera)->getDefaultSettingCount();
}

int ecCamGetCurrentResolution(ECOREOBJ camera)
{
    return ((V4LCamera *)camera)->getDefaultSettingCount();
}

void ecCamGetResolution(ECOREOBJ camera, int index, ECOREOBJ modein)
{
    E_CAMERAMODE * mode = (E_CAMERAMODE *) modein;
    
    if (index < ((V4LCamera *)camera)->getDefaultSettingCount())
    	((V4LCamera *)camera)->getDefaultSetting(index, *mode);
    else
        ((V4LCamera *)camera)->getCurrentSetting(*mode);
}

unsigned long ecCamSetResolution(ECOREOBJ camera, int index)
{
    E_CAMERAMODE mode;
    if (index < 0) return E_INVALIDPARAM;        
    if (index >= ((V4LCamera *)camera)->getDefaultSettingCount()) return E_INVALIDPARAM;
    ((V4LCamera *)camera)->getDefaultSetting(index, mode);
    int v4lformat = 0;
    for (int i = 0; i < 10; i++) if(dspformats[i] == mode.format) v4lformat = v4lformats[i];
    if (v4lformat == 0) return E_ERROR;
    return ((V4LCamera *)camera)->setResolution(mode.x, mode.y, v4lformat);
}


//
// Callbacks from edge - goes via polling rather than runloops or asynchronous callbacks
//

typedef struct camera_list
{
    ECOREOBJ camera;
    camera_list * next;    
} camera_list;

static camera_list * cameralist = NULL;

void ecCamPoll(void)
{
	camera_list * item = cameralist;
	while(item)
	{
		((V4LCamera *)item->camera)->poll();
		item = (camera_list*) (item->next);
	}
}

void ecCamRegister(ECOREOBJ camobj)
{
	camera_list ** headpt = &cameralist;
	camera_list * item = cameralist;
	while(item)
	{
		headpt = &(item->next);
		if (item->camera == camobj) return;
		item = (item->next);
	}
	*headpt = new camera_list;
	(*headpt)->next = NULL;
	(*headpt)->camera = camobj;
}

void ecCamUnregister(ECOREOBJ camobj)
{
	camera_list ** headpt = &cameralist;
	camera_list * item = cameralist;
	while(item)
	{
		if (item->camera == camobj)
		{
			*headpt = (camera_list *)(item->next);
			delete item;
			return;
		}
		headpt = &(item->next);
		item = item->next;
	}
}

