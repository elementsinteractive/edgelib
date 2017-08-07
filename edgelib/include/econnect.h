// econnect.h
// A module for setting up internet and wireless connections
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED      //Can only be included by the main engine
#ifndef ECONNECT_INCLUDED  //Make sure the module isn't included multiple times
#define ECONNECT_INCLUDED

#define ECONN_NONE           0
#define ECONN_SOCKET         1
#define ECONN_BLUETOOTH      2 //Connection types

#define EPING_NONE           0
#define EPING_WAITHOST       1
#define EPING_WAITREPLY      2 //Ping states

#define EHTTP_CONNECTION_NONE       0
#define EHTTP_CONNECTION_KEEPALIVE  1
#define EHTTP_CONNECTION_CLOSE      2 //HTTP connection settings

#define MAX_ECONREADBUF   1024 //Buffer for reading socket data
#define MAX_ECONCLIENTS     16 //Maximum number of clients to accept
#define MAX_MIMETYPES       64 //Maximum number of array elements for mime types

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

typedef struct
{
	char name[64];
	char address[18];
	unsigned char port;
}EBLUETOOTH_DEVICEID;
//A bluetooth device ID

typedef struct
{
	char devicename[64];
	char servicename[64];
	char address[18];
	unsigned char port;
}EBLUETOOTH_SERVICEID;
//A bluetooth device ID

typedef struct
{
	char contentencoding[16]; //not fully supported
	char contenttype[128];
	char server[256];
	char location[1024];
	char date[48];
	long contentlength;
	unsigned short status;
}EHTTPRESPONSE_INFO;
//An HTTP header

typedef struct
{
	bool acceptencoding_gzip; //not fully supported
	char host[512];
	char path[512];
	char referer[1024];
	char useragent[128];
	char username[128];
	char password[128];
	unsigned char connection;
	int port;
}EHTTPREQUEST_INFO;
//Info structure for an HTTP request

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

//Forward declarations
class ClassENetCapture;
class EHTTPRequest;

class ClassEConnect
{
	friend class ClassEdge;
	friend class EHTTPRequest;
	private:
		ClassENetCapture *capturedapi;
		ClassEdge *edge;
		ECOREOBJ netobj;
		ECOREOBJ clientsocket[MAX_ECONCLIENTS];
		ECOREOBJ opensocket;
		char *httpcommand;
		unsigned char readbuf[MAX_ECONREADBUF];
		unsigned char connectid;
		unsigned long httpcmdsize;
		unsigned long pingtimestamp;
		unsigned long maxsearchdevice;
		unsigned long cursearchdevice;
		unsigned short usedprotocol;
		unsigned char pingstate;
		bool connectopen;
		bool issearchingservice;
		bool isserver;
		bool isadvertising;
		bool btonce;
		bool tmpbton;
		void static SplitURL(const char *url, char *host, char *page, unsigned long urlend = 0xFFFFFFF);
		static inline bool IsBase64(unsigned char c);
		static void EventHandler(unsigned short eventtype, void *eventdata, void *userdata);
		void ExpandString(char *str, unsigned long &pos, const char *str2);
		void GenerateBoundary(char *boundary, unsigned char bsize = 40);
		void EdgeSend(unsigned char id, const unsigned char *buffer, long buflen, long &totallen, bool simulate);
		void SendPingData(unsigned char id, bool request);
		void LogStatus(const char *msg);
		void CallNetMessage(long id, long msg, long bufsize, unsigned char *buffer);
		void CheckSockets(void);
		void CheckBluetoothDevices(void);
		void OnDeviceFound(EBLUETOOTH_DEVICEID *device, bool endlist);
		void OnServiceFound(EBLUETOOTH_SERVICEID *service);
	public:
		ClassEConnect(void);
		~ClassEConnect(void);
		void SetConnectID(unsigned char id);
		ERESULT Init(unsigned short protocol, ClassEdge *pedge);
		void Cleanup(void);
		void AttachNetCapture(ClassENetCapture *netcapture);
		void DetachNetCapture(void);
		ERESULT EstablishConnection(void);
		void ReleaseConnection(void);
		unsigned long SearchWireless(unsigned long numdevices = 16, bool searchservices = false);
		void StopWirelessSearch(void);
		bool SearchHost(const char *address, int port);
		void Disconnect(unsigned char connection = 0);
		void Send(const unsigned char *buffer, long buflen, unsigned char connection = 0);
		void SendRaw(const unsigned char *buffer, long buflen, unsigned char connection = 0);
		bool SendHTTPGet(const char *url, bool autoencode = false);
		bool SendHTTPGet(EHTTPRequest *request);
		bool SendHTTPPost(const char *url, bool autoencode = false);
		bool SendHTTPPost(EHTTPRequest *request);
		bool SendHTTPPostMultipart(EHTTPRequest *request);
		void RequestPing(unsigned char connection = 0);
		void RespondPing(unsigned char connection = 0);
		unsigned long GetPingTime(void);
		void static UrlEncode(char *dst, const char *src);
		void static UrlDecode(char *dst, const char *src);
		void static Base64Encode(char *dst, const char *src);
		void static Base64Decode(char *dst, const char *src);
};//The connection class

class ENetStack
{
	private:
		bool peekcache;
		unsigned char *rcvstack;
		unsigned long stackindex;
		unsigned long totalstacksize;
		unsigned long curstacksize;
		unsigned long lastpeeksize;
		unsigned long lastpeekindex;
		void ScanEdge(unsigned long &pkgstart, unsigned long &pkgsize);
	public:
		ENetStack(void);
		~ENetStack(void);
		bool Alloc(unsigned long stacksize);
		void DeAlloc(void);
		bool Push(const void *buffer, unsigned long bufsize);
		unsigned long Peek(void);
		unsigned char Pop(void *buffer, unsigned long &bufsize);
};//The connection data stack class

class EHTTPStack
{
	private:
		EHTTPRESPONSE_INFO httpheader;
		unsigned char *rcvstack;
		unsigned long totalstacksize;
		unsigned long curstacksize;
		unsigned long stackindex;
		unsigned char lrcnt;
		unsigned char lncnt;
		bool compressedbody;
		void ParseHeader(void);
	public:
		EHTTPStack(void);
		~EHTTPStack(void);
		void Clear(void);
		bool Alloc(unsigned long stacksize);
		void DeAlloc(void);
		bool Push(void *buffer, unsigned long bufsize);
		unsigned long BodySize(void);
		bool GetBody(void *buffer, unsigned long &bufsize);
		//bool GetBodyUncompressed(void *buffer, unsigned long &bufsize);
		bool GetResponseInfo(EHTTPRESPONSE_INFO *info);
		bool GetResponseInfo(char *rawinfo);
};//The HTTP header stack class

class EHTTPRequest
{
	friend class ClassEConnect;
	private:
		unsigned long *paramvaluesize;
		unsigned char **paramvalue;
		char **paramfilename;
		char **paramname;
		char **mimetype;
		char *customheader;
		unsigned long paramcount;
		void DefaultData(void);
		bool ExpandList(unsigned char numblocks);
		void AddMimeArray(unsigned long &index, char extmime[MAX_MIMETYPES][8], char namemime[MAX_MIMETYPES][32], const char *ext, const char *name);
		void GetMimeType(char *mime, const char *filename);
		void GetMimeType(char *mime, const WCHAR *filename);
	public:
		EHTTPREQUEST_INFO info;
		EHTTPRequest(void);
		EHTTPRequest(const char *seturl);
		~EHTTPRequest(void);
		bool AddParam(const char *name, const char *value);
		bool AddParam(const char *name, long value);
		bool AddData(const char *name, const unsigned char *buffer, unsigned long bufsize, const char *filename = NULL, const char *datamimetype = NULL);
		bool AddFile(const char *name, const char *filename, const char *filemimetype = NULL);
		bool AddFile(const char *name, ClassEFile *ef, const char *filemimetype = NULL);
		void AddCustomHeader(const char *hdr);
};//The HTTP request class

#endif
#endif
