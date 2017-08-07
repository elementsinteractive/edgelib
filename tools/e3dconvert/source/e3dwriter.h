// e3dwriter.h
// Writes E3D files to disk
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Type definitions                                              //
///////////////////////////////////////////////////////////////////

//Stream data chunk size
#define E3DF_MAXSTREAMDAT  1024

//Macro for 4-character codes
#define E3DF_FOURCC(c1, c2, c3, c4) (c1 | (c2 << 8) | (c3 << 16) | (c4 << 24))

//File identifier
#define E3DF_SIGNATURE E3DF_FOURCC('E', '3', 'D', '_')
#define E3DF_EIBV      E3DF_FOURCC('E', 'I', 'B', 'V')
#define E3DF_VER       E3DF_FOURCC('1', '.', '0', '0')

//Chunks
#define E3DCH_eGRP    E3DF_FOURCC('e', 'G', 'R', 'P')
#define E3DCH_EMSH    E3DF_FOURCC('E', 'M', 'S', 'H')
	#define E3DCH_EVTX    E3DF_FOURCC('E', 'V', 'T', 'X')
	#define E3DCH_ETRI    E3DF_FOURCC('E', 'T', 'R', 'I')
	#define E3DCH_eCOL    E3DF_FOURCC('e', 'C', 'O', 'L')
	#define E3DCH_eNRM    E3DF_FOURCC('e', 'N', 'R', 'M')
	#define E3DCH_eTXC    E3DF_FOURCC('e', 'T', 'X', 'C')
	#define E3DCH_eJNT    E3DF_FOURCC('e', 'J', 'N', 'T')
	#define E3DCH_eKEY    E3DF_FOURCC('e', 'K', 'E', 'Y')
#define E3DCH_END     E3DF_FOURCC('_', 'E', 'N', 'D')

//Content flags
#define E3CFL_VERTICES     0x1
#define E3CFL_TRIANGLES    0x2
#define E3CFL_COLORS       0x4
#define E3CFL_NORMALS      0x8
#define E3CFL_TEXCOORDS   0x10
#define E3CFL_JOINTS      0x20
#define E3CFL_KEYFRAMES   0x40

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

//3D model structure
typedef struct
{
	unsigned long type;
	unsigned long edgelib[2];
}E3DF_HEADER;

//3D model structure
typedef struct
{
	char name[32];
	unsigned long contentflags;
	unsigned long vertices;
	unsigned long triangles;
}E3DF_MODEL;

///////////////////////////////////////////////////////////////////
// Writer
///////////////////////////////////////////////////////////////////

//Wegschrijven bestand
bool WriteE3D(E3DSurface *surf, E3DSURFACEINFO *info, char *outputfile)
{
	ClassEFile ef;
	if (info->vertexarray == NULL || info->indexarray == NULL || info->useindices == false)
		return(false);
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
		if (info->colorarray)
			model.contentflags |= E3CFL_COLORS;
		if (info->normalarray)
			model.contentflags |= E3CFL_NORMALS;
		if (info->texcoordarray)
			model.contentflags |= E3CFL_TEXCOORDS;
		model.vertices = surf->VerticesInUse();
		model.triangles = surf->PolygonsInUse();
		ef.Write(&model, sizeof(model));
		
		//Vertex array
		val = E3DCH_EVTX;
		ef.Write(&val, 4);
		val = model.vertices * 12;
		ef.Write(&val, 4);
		ef.Write(info->vertexarray, val);

		//Index array
		val = E3DCH_ETRI;
		ef.Write(&val, 4);
		val = model.triangles * 6;
		ef.Write(&val, 4);
		ef.Write(info->indexarray, val);

		//Color array
		if (model.contentflags & E3CFL_COLORS)
		{
			val = E3DCH_eCOL;
			ef.Write(&val, 4);
			val = model.vertices * 4;
			ef.Write(&val, 4);
			ef.Write(info->colorarray, val);
		}

		//Normal array
		if (model.contentflags & E3CFL_NORMALS)
		{
			signed char *nrm_mem = (signed char *)ClassEMemory::Alloc(model.vertices * 3);
			if (nrm_mem)
			{
				signed char *nrmarr = (signed char *)info->normalarray;
				for (val = 0; val < model.vertices; val++)
				{
					nrm_mem[val * 3] = nrmarr[val * 4];
					nrm_mem[val * 3 + 1] = nrmarr[val * 4 + 1];
					nrm_mem[val * 3 + 2] = nrmarr[val * 4 + 2];
				}
				val = E3DCH_eNRM;
				ef.Write(&val, 4);
				val = model.vertices * 3;
				ef.Write(&val, 4);
				ef.Write(nrm_mem, val);
				ClassEMemory::DeAlloc(nrm_mem);
			}
		}

		//Normal array
		if (model.contentflags & E3CFL_TEXCOORDS)
		{
			long *texarr = (long *)info->texcoordarray;
			for (val = 0; val < model.vertices; val++)
				texarr[val * 2 + 1] = -texarr[val * 2 + 1];
			val = E3DCH_eTXC;
			ef.Write(&val, 4);
			val = model.vertices * 8;
			ef.Write(&val, 4);
			ef.Write(info->texcoordarray, val);
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
