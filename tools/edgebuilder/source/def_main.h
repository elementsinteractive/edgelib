// def_main.h
// Main program definitions
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////

//General definitions
#define PBO_CAPTION           "EDGELIB Builder (beta 17)"

//Limits
#define MAX_PLATFORMS        32
#define MAX_SOURCEFILES    1024
#define MAX_HEADERFILES       MAX_SOURCEFILES
#define MAX_RESOURCEFILES     MAX_SOURCEFILES
#define MAX_SOURCEFOLDERS    16
#define MAX_HEADERFOLDERS     MAX_SOURCEFOLDERS
#define MAX_RESOURCEFOLDERS   MAX_SOURCEFOLDERS
#define MAX_TREETYPES        16

//Target types
#define TARGET_EXE            0
#define TARGET_LIB            1

//Window timers
#define TMR_AUTOBUILD         5
#define TMR_AUTOEXECUTE       6
#define TMR_AUTOEXIT          7

//Build thread commands
#define BTC_NONE              0
#define BTC_COMPILE           1
#define BTC_LINK              2
#define BTC_BUILD             3
#define BTC_REBUILDALL        4
#define BTC_EXECUTE           5
#define BTC_BUILDINSTALL      6

//Return values
#define RVAL_OK               0
#define RVAL_ERRCOMPILE       1
#define RVAL_ERRRESOURCE      2
#define RVAL_ERRLINK          3
#define RVAL_ERRINSTALL       4
#define RVAL_ERRCMDLINE     100

/////////////////////////////////////////////////////////////////////
// Capability string array
/////////////////////////////////////////////////////////////////////

const char capability_str[][16] =
{
	"LocalServices",
	"UserEnvironment",
	"NetworkServices",
	"ReadUserData",
	"WriteUserData",
	"",
	"",
	"",
	"ReadDeviceData",
	"WriteDeviceData",
	"SWEvent",
	"ProtSrv",
	"SurroundingsDD",
	"TrustedUI",
	"PowerMgmt",
	"Location",
	"DRM",
	"MultimediaDD",
	"Network Control",
	"TCB",
	"AllFiles",
	"CommDD",
	"DiskAdmin",
	""
};

const char fwdependency_str_short[][16] =
{
	"opengles",
	"agpaudio",
	"oggvorbis",
	"",
	"",
	"",
	"",
	""
};

const char fwdependency_str_long[][128] =
{
	"http://components.antixlabs.com/if/opengles 1.1-1.*",
	"http://components.antixlabs.com/if/agpaudio 1.*",
	"http://components.antixlabs.com/if/codec/audio/ogg-vorbis 1.*",
	"",
	"",
	"",
	"",
	""
};

/////////////////////////////////////////////////////////////////////
// Structures
/////////////////////////////////////////////////////////////////////

class ClassCompiler;
class ClassMain;

//Dragging info for the treeview control
typedef struct
{
	HTREEITEM lasttarget;
	HTREEITEM itemhandle;
	unsigned char itemtype;
	unsigned long itemid;
	bool active;
}TREEVIEW_DRAGINFO;

//An source or header file in an Edge project
typedef struct
{
	char file[MAX_EFILESTR];
	char path[MAX_EFILESTR];
	unsigned short parent;
	bool treeadded;
}PRJFILE;

//An header file in an Edge project
typedef struct
{
	HTREEITEM handle;
	char name[MAX_ESTR];
	unsigned short parent;
	bool treeadded;
	bool expanded;
}PRJFOLDER;

//An Edge project
typedef struct
{
	class ClassEnv *env;
	PRJFILE source[MAX_SOURCEFILES];
	PRJFILE header[MAX_HEADERFILES];
	PRJFILE resource[MAX_RESOURCEFILES];
	PRJFOLDER sourcefld[MAX_SOURCEFOLDERS];
	PRJFOLDER headerfld[MAX_HEADERFOLDERS];
	PRJFOLDER resourcefld[MAX_RESOURCEFOLDERS];
	long buildreturn;
	unsigned long appuid;
	unsigned short numsourcefiles;
	unsigned short numsourcefolders;
	unsigned short numheaderfiles;
	unsigned short numheaderfolders;
	unsigned short numresourcefiles;
	unsigned short numresourcefolders;
	unsigned char capabilities[3];
	unsigned char fwdependency;
	unsigned char targettype;
	char headerloc[MAX_SOURCEFILES][MAX_EFILESTR];
	char loadfolder[MAX_EFILESTR];
	char loadfile[MAX_EFILESTR];
	char name[MAX_ESTR];
	char caption[MAX_ESTR];
	char vendor[MAX_ESTR];
	char vendorshort[MAX_ESTR];
	char domain[MAX_ESTR];
	char pathoverride[MAX_ESTR];
	char vermajor[8];
	char verminor[8];
	char buildnr[16];
	bool expandtreetype[MAX_TREETYPES];
	bool selectplatform[MAX_PLATFORMS];
	bool noresemu;
	bool loaded;
	bool updated;
	bool cancelbuild;
	bool dopathoverride;
}EDGEPROJECT;

//An Edge IDE compiler platform
typedef struct
{
	class ClassEnv *env;
	char name[MAX_ESTR];
	char shortname[MAX_ESTR];
	char reloadfile[MAX_ESTR];
	bool legacyuid;
	bool linuxpath;
}EDGEPLATFORM;

//Information on the current window
typedef struct
{
	long leftspacing;
	long rightspacing;
	long topspacing;
	long bottomspacing;
	long subspacing;
	long outputheight;
}GUIINFO;

//Thread build info
typedef struct
{
	char exefile[MAX_EFILESTR];
	ClassMain *classmain;
	long param;
	unsigned short platform;
	unsigned char command;
}BUILDTHREADINFO;

//Commandline info
typedef struct
{
	char useplatform[MAX_ESTR];
	bool autobuild;
	bool autosetup;
	bool showhelp;
}AUTOCMDINFO;

//Program settings
typedef struct
{
	char prjnewvendor[MAX_ESTR];
	char prjnewvendorshort[MAX_ESTR];
	bool buildbatch;
}PRGSETTINGS;
