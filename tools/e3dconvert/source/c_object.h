// c_object.h
// Object container class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#define MAX_MESHES  1

//Container class for 3D data
class ObjectContainer : public E3DLoaderBase
{
	public:
		MeshContainer mesh[MAX_MESHES];
		unsigned char curmesh;
	public:
		ObjectContainer(void);
		~ObjectContainer(void);
		ERESULT LdrOnMeshStart(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnMeshEnd(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnVertexStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnNormalStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnTexcoordStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnColorStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnIndexStream(ELDR_3DCALLBACKINFO *ldrdata);
};

///////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////

//Constructor
ObjectContainer::ObjectContainer(void)
{
	curmesh = 0;
}

//Destructor
ObjectContainer::~ObjectContainer(void)
{
}

//Callback: start new mesh
ERESULT ObjectContainer::LdrOnMeshStart(ELDR_3DCALLBACKINFO *ldrdata)
{
	mesh[curmesh].LdrOnMeshStart(ldrdata);
	return(E_OK);
}

//Callback: end mesh
ERESULT ObjectContainer::LdrOnMeshEnd(ELDR_3DCALLBACKINFO *ldrdata)
{
	mesh[curmesh].LdrOnMeshEnd(ldrdata);
	curmesh++;
	if (curmesh >= MAX_MESHES)
		return(E_ABORT);
	return(E_OK);
}

//Callback: vertex stream
ERESULT ObjectContainer::LdrOnVertexStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	return(mesh[curmesh].LdrOnVertexStream(ldrdata));
}

//Callback: normal stream
ERESULT ObjectContainer::LdrOnNormalStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	return(mesh[curmesh].LdrOnNormalStream(ldrdata));
}

//Callback: texture coordinate stream
ERESULT ObjectContainer::LdrOnTexcoordStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	return(mesh[curmesh].LdrOnTexcoordStream(ldrdata));
}

//Callback: color stream
ERESULT ObjectContainer::LdrOnColorStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	return(mesh[curmesh].LdrOnColorStream(ldrdata));
}

//Callback: triangle index stream
ERESULT ObjectContainer::LdrOnIndexStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	return(mesh[curmesh].LdrOnIndexStream(ldrdata));
}
