// parsesdp.h
// Windows low-level SDP parser for Bluetooth records
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//SDP service record test & layout
/*
35                                              start SDP record
4A                                              SDP record size - 2

09 00 01                                        Service Class ID List 
35 11                                           Data sequence 0x11 (17) bytes
1C                                              ? long UID identifier ?
00 00 11 01 00 00 10 00 80 00 00 80 5F 9B 34 FB Service Class UID

09 00 04                                        Protocol descriptor list
35 0C                                           Data sequence 0x0C (12) bytes
35 03                                           Protocol ID 3 bytes
19 01 00                                        L2CAP_PROTOCOL_UUID16 [0x0100] 
35 05                                           Protocol ID 5 bytes
19 00 03 08 00                                  RFCOMM_PROTOCOL_UUID16 [0x0003], port 0x00

09 00 06                                        Language attribute list
35 09                                           Data sequence 9 bytes
09 65 6E                                        English
09 00 6A
09 01 00

09 01 00                                        Service name
25 10                                           Text string 0x10 (16) bytes
45 64 67 65 20 53 65 72 69 61 6C 20 50 6F 72 74 "Edge serial port"
*/

//Generic definitions
#define MAX_PARSESDPRECORD    256

//Parsed SDP data structure
typedef struct
{
	char servicename[64];
	unsigned char port;
	bool p_l2cap;
	bool p_rfcomm;
}ESDPRECORDINFO;

class CEdgeParseSDP
{
	public:
		unsigned char blob[MAX_PARSESDPRECORD];
		unsigned long bloblength;
		CEdgeParseSDP(void);
		~CEdgeParseSDP(void);
		void SetBlob(unsigned char *newblob, unsigned long blobsize);
		void SetClassGuid(unsigned char *guid);
		void SetSerialPort(unsigned char port);
		void SetName(char *name);
		void SetUnknownList(void);
		void ParseBlob(ESDPRECORDINFO *sdpinfo);
};//Edge SDP parser class

///////////////////////////////////////////////////////////////////
// Class implementation                                          //
///////////////////////////////////////////////////////////////////

//CEdgeParseSDP: constructor
CEdgeParseSDP::CEdgeParseSDP(void)
{
	ecMemSet(blob, 0, MAX_PARSESDPRECORD);
	blob[0] = 0x35;
	bloblength = 2;
}

//CEdgeParseSDP: destructor
CEdgeParseSDP::~CEdgeParseSDP(void)
{
}

//Set an existing blob object
void CEdgeParseSDP::SetBlob(unsigned char *newblob, unsigned long blobsize)
{
	ecMemCpy(blob, newblob, blobsize);
	bloblength = blobsize;
}

//Set the unique class UID
void CEdgeParseSDP::SetClassGuid(unsigned char *guid)
{
	unsigned char dscl[] = {
		0x09, 0x00, 0x01, //List: service class ID
		0x35, 0x11,       //17 bytes
		0x1C              //128-bit UUID
	};
	ecMemCpy(&blob[bloblength], dscl, sizeof(dscl));
	bloblength += sizeof(dscl);
	ecMemCpy(&blob[bloblength], guid, 16);
	bloblength += 16;
	blob[1] = (unsigned char)(bloblength - 2);
}

//Set the serial port protocol, specify the used port
void CEdgeParseSDP::SetSerialPort(unsigned char port)
{
	unsigned char dscl[] = {
		0x09, 0x00, 0x04,            //List: protocol descriptor
		0x35, 0x0C,                  //12 bytes
		0x35, 0x03,                  //3 bytes
		0x19, 0x01, 0x00,            //L2CAP
		0x35, 0x05,                  //5 bytes
		0x19, 0x00, 0x03, 0x08, 0x00 //RFCOMM, port (0 << replace)
	};
	dscl[sizeof(dscl) - 1] = port;
	ecMemCpy(&blob[bloblength], dscl, sizeof(dscl));
	bloblength += sizeof(dscl);
	blob[1] = (unsigned char)(bloblength - 2);
}

//Set the name of the service
void CEdgeParseSDP::SetName(char *name)
{
	unsigned char dscl[] = {
		0x09, 0x01, 0x00, //List: service name
		0x25, 0x00        //SDP string (0 << replace)
	};
	dscl[sizeof(dscl) - 1] = (unsigned char)ecStrLen(name);
	ecMemCpy(&blob[bloblength], dscl, sizeof(dscl));
	bloblength += sizeof(dscl);
	ecMemCpy(&blob[bloblength], name, ecStrLen(name));
	bloblength += ecStrLen(name);
	blob[1] = (unsigned char)(bloblength - 2);
}

//Add a descriptor list (currently unidentified)
void CEdgeParseSDP::SetUnknownList(void)
{
	unsigned char dscl[] = {
		0x09, 0x00, 0x06, //List: ???
		0x35, 0x09,       //9 bytes
		0x09, 0x65, 0x6E, 0x09, 0x00, 0x6A, 0x09, 0x01, 0x00 //???
	};
	ecMemCpy(&blob[bloblength], dscl, sizeof(dscl));
	bloblength += sizeof(dscl);
	blob[1] = (unsigned char)(bloblength - 2);
}

//Parse blob data
void CEdgeParseSDP::ParseBlob(ESDPRECORDINFO *sdpinfo)
{
	/*
	char fname[512];
	ClassEStd::StrCpy(fname, "_service");
	ClassEStd::StrCat(fname, bloblength);
	ClassEStd::StrCat(fname, ".dat");
	ClassEFile ef;
	ef.New(fname);
	ef.Write(blob, bloblength);
	ef.Close();
	*/
	unsigned long blobpos = 2;
	sdpinfo->port = 0;
	ecStrCpy(sdpinfo->servicename, "");
	sdpinfo->p_l2cap = false;
	sdpinfo->p_rfcomm = false;
	while(blobpos < bloblength)
	{
		if (blob[blobpos] == 9) //Start list
		{
			unsigned short listtype = blob[blobpos + 1] * 256 + blob[blobpos + 2];
			blobpos += 3;
			if (listtype == 0x0000) //Unknown, initializer or default/empty?
			{
				blobpos += 5;
			}
			else if (listtype == 0x0004) //Protocol descriptor
			{
				unsigned char *b_prot = &blob[blobpos];
				if (b_prot[0] == 0x35)
				{
					unsigned long prot_pos = 0;
					while (prot_pos < b_prot[1])
					{
						if (b_prot[prot_pos + 2] == 0x35)
						{
							unsigned long prot_size = b_prot[prot_pos + 3];
							prot_pos += 2;
							if (prot_size >= 3)
							{
								unsigned long protocoltype = b_prot[prot_pos + 2] * 65536 + b_prot[prot_pos + 3] * 256 + b_prot[prot_pos + 4];
								if (protocoltype == 0x190100) //L2CAP
								{
									sdpinfo->p_l2cap = true;
								}
								else if (protocoltype == 0x190003 && prot_size == 5)
								{
									sdpinfo->p_rfcomm = true;
									sdpinfo->port = b_prot[prot_pos + 6];
								}
							}
							prot_pos += prot_size;
						}
						else
							break;
					}
					blobpos += b_prot[1] + 2;
				}
			}
			else if (listtype == 0x0100) //Service name
			{
				if (blob[blobpos] == 0x25 && blob[blobpos + 1] < 64) //Text string < 64 characters
				{
					ecMemSet(sdpinfo->servicename, 0, 64);
					ecMemCpy(sdpinfo->servicename, &blob[blobpos + 2], blob[blobpos + 1]);
					blobpos += blob[blobpos + 1] + 2;
				}
				else
					break;
			}
			else if (blob[blobpos] == 0x35)
				blobpos += blob[blobpos + 1] + 2;
			else if (blob[blobpos] == 0x0A)
				blobpos += 5;
			else
				break;
		}
		else
			break;
	}
}
