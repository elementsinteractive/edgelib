// app.cpp
// E3DConv: Convert various 3D formats to internal E3D format
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Link Edge
#pragma comment(lib, "edge.lib")
#include "edgemain.h"

//Windows headers
#include <direct.h>
#include <stdio.h>

//Global settings
bool addcolors = true;
bool addnormals = true;
bool addtexels = true;
signed char addresize = 0;

#include "c_mesh.h"
#include "c_object.h"
#include "e3dwriter.h"

#define MAX_INPUTFILE 32
#define MAX_GFXLOADER  8

//Check de commandline en lees parameters uit
int checkcommandline(char *cmdline, char *inputfiles, char *outputfolder)
{
	char cmdarg[MAX_ESTR];
	bool onquote = false, onparam = false;
	unsigned char onp = 0, onfile = 0;
	unsigned long ctr, startp = 0, len = strlen(cmdline);
	for (ctr = 0; ctr < MAX_INPUTFILE; ctr++)
		inputfiles[ctr * MAX_EFILESTR] = 0;
	for (ctr = 0; ctr < len; ctr++)
	{
		if (!onparam && cmdline[ctr] != ' ')
		{
			startp = ctr;
			onparam = true;
		}
		if (onparam)
		{
			if (cmdline[ctr] == '\"')
				onquote = !onquote;
			if ((!onquote && cmdline[ctr] == ' ') || ctr == len - 1)
			{
				if (onp > 0)
				{
					bool quotearg = false;
					memcpy(cmdarg, &cmdline[startp], ctr - startp + 1);
					cmdarg[ctr - startp + 1] = 0;
					ClassEStd::StrTrim(cmdarg);
					if (cmdarg[0] == '"' && cmdarg[strlen(cmdarg) - 1] == '"')
					{
						quotearg = true;
						cmdarg[0] = ' ';
						cmdarg[strlen(cmdarg) - 1] = ' ';
					}
					ClassEStd::StrTrim(cmdarg);
					if (!quotearg && (cmdarg[0] == '-' || cmdarg[0] == '/'))
					{
						if (ClassEStd::StrEqual(&cmdarg[1], "nc", false))
						{
							addcolors = false;
						}
						else if (ClassEStd::StrEqual(&cmdarg[1], "nn", false))
						{
							addnormals = false;
						}
						else if (ClassEStd::StrEqual(&cmdarg[1], "nt", false))
						{
							addtexels = false;
						}
						else if ((cmdarg[1] == 's' || cmdarg[1] == 'S') && cmdarg[2] == ':')
						{
							addresize = ClassEStd::StrToInt(&cmdarg[3]);
						}
						else if ((cmdarg[1] == 'd' || cmdarg[1] == 'D') && cmdarg[2] == ':')
						{
							ClassEStd::StrCpy(outputfolder, &cmdarg[3]);
						}
						else if (ClassEStd::StrEqual(&cmdarg[1], "?", false) || ClassEStd::StrEqual(&cmdarg[1], "help", false))
						{
							onp = 1;
							break;
						}
						else
						{
							printf("Unknown option: ");
							printf(cmdarg);
							printf("\n");
							return(-3);
						}
					}
					else
					{
						strcpy(&inputfiles[onfile * MAX_EFILESTR], cmdarg);
						onfile++;
					}
				}
				onparam = false;
				onp++;
			}
		}
	}
	if (onp == 0)
	{
		printf("e3dconv: Command error\n");
		return(-1);
	}
	else if (onp == 1)
	{
		printf("usage: e3dconv [options] [inputfiles]\n");
		printf("\nSupported options:\n");
		printf("-s:size: resize model, negative values to downsize, positive values to upsize\n");
		printf("-d:destination: destination path to write the converted file to\n");
		printf("-nc: Don't add color information\n");
		printf("-nn: Don't add normal information\n");
		printf("-nt: Don't add texture coordinates\n");
		printf("\nSupported formats:\n");
		printf("3D Studio (3DS)\n");
		printf("Milkshape 3D (MS3D)\n");
		return(-2);
	}
	else
	{
		if (strlen(inputfiles) == 0)
		{
			printf("Error: no input file specified\n");
			return(-4);
		}
	}
	return(0);
}

//Start loading of image
ERESULT StartLoading(void *fdata, unsigned long fsize, signed char addresize, ObjectContainer *object)
{
	ClassELoaderBase *graphicsloader[MAX_GFXLOADER];
	ERESULT er = E_UNSUPPORTED;
	unsigned char ctr, ldridx = 0;
	for (ctr = 0; ctr < MAX_GFXLOADER; ctr++)
		graphicsloader[ctr] = NULL;
	ELoader_3DS *l3ds = new ELoader_3DS;
	if (l3ds != NULL)
		graphicsloader[ldridx++] = l3ds;
	ELoader_MS3D *ms3d = new ELoader_MS3D;
	if (ms3d != NULL)
		graphicsloader[ldridx++] = ms3d;
	ELoader_E3D *e3d = new ELoader_E3D;
	if (e3d != NULL)
		graphicsloader[ldridx++] = e3d;
	ELoader_DAE *dae = new ELoader_DAE;
	if (dae != NULL)
		graphicsloader[ldridx++] = dae;
	for (ctr = 0; ctr < ldridx; ctr++)
	{
		er = graphicsloader[ctr]->Start(object, (unsigned char *)fdata, fsize, addresize, 0);
		if (er != E_UNSUPPORTED && er != E_NOFEATURE)
			break;
	}
	for (ctr = 0; ctr < ldridx; ctr++)
		if (graphicsloader[ctr] != NULL)
			delete graphicsloader[ctr];
	return(er);
}

//Wegschrijven bestand
bool WriteDestFile(ObjectContainer *object, char *outputfile)
{
	ClassEFile ef;
	if (ef.New(outputfile))
	{
		unsigned long val;

		//Signature
		val = E3DF_SIGNATURE;
		ef.Write(&val, 4);
		val = E3DF_EIBV;
		ef.Write(&val, 4);
		val = E3DF_VER;
		ef.Write(&val, 4);

		//Model
		E3DF_MODEL model;
		val = E3DCH_EMSH;
		ef.Write(&val, 4);
		val = sizeof(model);
		ef.Write(&val, 4);
		ClassEStd::StrCpy(model.name, "3D Model");
		model.contentflags = E3CFL_VERTICES | E3CFL_TRIANGLES;
		//if (info->colorarray)
		//	model.contentflags |= E3CFL_COLORS;
		//if (info->normalarray)
			model.contentflags |= E3CFL_NORMALS;
		//if (info->texcoordarray)
			model.contentflags |= E3CFL_TEXCOORDS;
		model.vertices = object->mesh[0].allocvtx;
		model.triangles = object->mesh[0].allocidx;
		ef.Write(&model, sizeof(model));
		
		//Vertex array
		val = E3DCH_EVTX;
		ef.Write(&val, 4);
		val = model.vertices * 12;
		ef.Write(&val, 4);
		ef.Write(object->mesh[0].dvtx, val);

		//Index array
		val = E3DCH_ETRI;
		ef.Write(&val, 4);
		val = model.triangles * 6;
		ef.Write(&val, 4);
		ef.Write(object->mesh[0].didx, val);

		//Color array
		/*if (model.contentflags & E3CFL_COLORS)
		{
			val = E3DCH_eCOL;
			ef.Write(&val, 4);
			val = model.vertices * 4;
			ef.Write(&val, 4);
			ef.Write(info->colorarray, val);
		}*/

		//Normal array
		if (model.contentflags & E3CFL_NORMALS)
		{
			val = E3DCH_eNRM;
			ef.Write(&val, 4);
			val = model.vertices * 3;
			ef.Write(&val, 4);
			ef.Write(object->mesh[0].dnorm, val);
		}

		//Texture coordinate array
		if (model.contentflags & E3CFL_TEXCOORDS)
		{
			long *texarr = (long *)object->mesh[0].dtex;
			for (val = 0; val < model.vertices; val++)
				texarr[val * 2 + 1] = -texarr[val * 2 + 1];
			val = E3DCH_eTXC;
			ef.Write(&val, 4);
			val = model.vertices * 8;
			ef.Write(&val, 4);
			ef.Write(object->mesh[0].dtex, val);
		}

		//End signature/close file
		val = E3DCH_END;
		ef.Write(&val, 4);
		ef.Close();
	}
	else
	{
		printf("Error: couldn't create output file");
		return(false);
	}
	return(true);
}

//Convert to E3D
int ConvertFiles(char *inputfile, char *outputfolder)
{
	if (ClassEStd::StrEqual(inputfile, "*.*", false) || ClassEStd::StrEqual(inputfile, "*.ms3d", false) || ClassEStd::StrEqual(inputfile, "*.3ds", false))
	{
		ClassEFile folf;
		char curfolder[MAX_EFILESTR];
		_getcwd(curfolder, MAX_EFILESTR);
		folf.SetCurrentFolder(curfolder);
		if (folf.OpenFolder("", inputfile))
		{
			while(folf.ReadFolder(curfolder))
			{
				int lasterror = 0;
				if (ClassEStd::StrFind(curfolder, ".ms3d", false) >= 0 || ClassEStd::StrFind(curfolder, ".3ds", false) >= 0)
					lasterror = ConvertFiles(curfolder, outputfolder);
				if (lasterror != 0)
				{
					folf.CloseFolder();
					return(lasterror);
				}
				
			}
			folf.CloseFolder();
		}
		return(0);
	}

	//New code
	ClassEFile ef;
	void *fdata;
	unsigned long ctr, fsize;
	char convertfile[MAX_EFILESTR];
	if (inputfile[1] == ':' || inputfile[0] == '\\' || inputfile[0] == '/')
	{
		strcpy(convertfile, inputfile);
	}
	else
	{
		_getcwd(convertfile, MAX_EFILESTR);
		strcat(convertfile, "\\");
		strcat(convertfile, inputfile);
	}
	fsize = ef.Size(convertfile);
	if (fsize)
	{
		fdata = ClassEMemory::Alloc(fsize);
		if (fdata)
		{
			if (ef.Open(convertfile, EFOF_READ))
			{
				ef.Read(fdata, fsize);
				ef.Close();
			}
			ObjectContainer object;
			ERESULT er = StartLoading(fdata, fsize, addresize, &object);
			ClassEMemory::DeAlloc(fdata);
			if (er == E_OK)
			{
				unsigned long dotpos = 0;
				if (strlen(outputfolder) > 0)
				{
					char orgconvert[MAX_EFILESTR];
					strcpy(orgconvert, convertfile);
					if (outputfolder[1] == ':' || outputfolder[0] == '\\' || outputfolder[0] == '/')
					{
						strcpy(convertfile, outputfolder);
					}
					else
					{
						_getcwd(convertfile, MAX_EFILESTR);
						strcat(convertfile, "\\");
						strcat(convertfile, outputfolder);
					}
					if (strlen(convertfile))
						if (convertfile[strlen(convertfile) - 1] != '\\' && convertfile[strlen(convertfile) - 1] != '/')
							strcat(convertfile, "\\");
					strcat(convertfile, ClassEFile::GetFileName(orgconvert));
				}
				for (ctr = strlen(convertfile) - 1; ctr >= 0; ctr--)
					if (convertfile[ctr] == '.')
					{
						dotpos = ctr;
						break;
					}
				strcpy(&convertfile[dotpos], ".e3d");
				if (!WriteDestFile(&object, convertfile))
					return(-8);
				return(0);
			}
			return(-7);
		}
		else
		{
			printf("Error: Not enough memory");
			return(-6);
		}
	}
	else
	{
		printf("Error: Couldn't read file(s)");
		return(-5);
	}
	

	//Old code
	/*
	ECD ecd;
	ClassEMemory mem;
	ClassEFile ef;
	ClassEMath math;
	ClassEDisplay display;
	E3DSurface surf;
	math.Init();
	ecd.mem = &mem;
	ecd.file = &ef;
	ecd.math = &math;
	ecd.dsp = &display;
	RegisterDisplayLoaders(&display);
	display.Open(NULL, NULL, &ecd);
	char convertfile[MAX_EFILESTR];
	long ctr;
	unsigned long exflags = 0;
	if (!addcolors)
		exflags |= EST_NOCOLORARRAY;
	if (!addnormals)
		exflags |= EST_NONORMALARRAY;
	if (!addtexels)
		exflags |= EST_NOTEXCOORDARRAY;
	if (inputfile[1] == ':' || inputfile[0] == '\\' || inputfile[0] == '/')
	{
		strcpy(convertfile, inputfile);
	}
	else
	{
		_getcwd(convertfile, MAX_EFILESTR);
		strcat(convertfile, "\\");
		strcat(convertfile, inputfile);
	}
	if (display.CreateSurface(&surf, convertfile, addresize, EST_NOINTERLEAVE | exflags) == E_OK)
	{
		E3DSURFACEINFO info;
		if (surf.Lock(&info))
		{
			unsigned long dotpos = 0;
			if (strlen(outputfolder) > 0)
			{
				char orgconvert[MAX_EFILESTR];
				strcpy(orgconvert, convertfile);
				if (outputfolder[1] == ':' || outputfolder[0] == '\\' || outputfolder[0] == '/')
				{
					strcpy(convertfile, outputfolder);
				}
				else
				{
					_getcwd(convertfile, MAX_EFILESTR);
					strcat(convertfile, "\\");
					strcat(convertfile, outputfolder);
				}
				if (strlen(convertfile))
					if (convertfile[strlen(convertfile) - 1] != '\\' && convertfile[strlen(convertfile) - 1] != '/')
						strcat(convertfile, "\\");
				strcat(convertfile, ClassEFile::GetFileName(orgconvert));
			}
			for (ctr = strlen(convertfile) - 1; ctr >= 0; ctr--)
				if (convertfile[ctr] == '.')
				{
					dotpos = ctr;
					break;
				}
			strcpy(&convertfile[dotpos], ".e3d");
			WriteE3D(&surf, &info, convertfile);
			surf.Unlock();
		}
		else
		{
			printf("Error: Couldn't get 3D data");
			return(-11);
		}
	}
	else
	{
		printf("Error: Couldn't read file(s)");
		return(-10);
	}*/
	return(0);
}

//Entry point
ClassEdge *EdgeMain(EDGESTARTUP *data){ return(NULL); } //Edge dummy
int main(int argc, char argv[])
{
	int result = 0;
	char cmdlinestr[8192];
	char inputfiles[MAX_INPUTFILE * MAX_EFILESTR];
	char outputfolder[MAX_EFILESTR];
	strcpy(outputfolder, "");
	strcpy(cmdlinestr, GetCommandLine());
	result = checkcommandline(cmdlinestr, inputfiles, outputfolder);
	if (result == 0)
	{
		unsigned char inpctr = 0;
		while(inpctr < MAX_INPUTFILE && inputfiles[inpctr * MAX_EFILESTR])
		{
			result = ConvertFiles(&inputfiles[inpctr * MAX_EFILESTR], outputfolder);
			inpctr++;
		}
	}
	return(result);
}
