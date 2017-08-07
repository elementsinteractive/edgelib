// filetree.h
// Handles file tree
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

#define NUM_TREEICONS     10

#define TI_FOLDEROPEN      0
#define TI_FOLDERCLOSE     1
#define TI_RESFOLDEROPEN   2
#define TI_RESFOLDERCLOSE  3
#define TI_SOURCEFILE      4
#define TI_HEADERFILE      5
#define TI_RESOURCEFILE    6
#define TI_PROJECT         7
#define TI_FOLDERROOT      8
#define TI_RESOURCEROOT    9

#define TT_NONE            0
#define TT_ROOT            1
#define TT_SOURCEROOT      2
#define TT_HEADERROOT      3
#define TT_RESOURCEROOT    4
#define TT_SOURCEFOLDER    5
#define TT_HEADERFOLDER    6
#define TT_RESOURCEFOLDER  7
#define TT_SOURCEFILE      8
#define TT_HEADERFILE      9
#define TT_RESOURCEFILE   10

//Class definition
class ClassFileTree
{
	private:
		HWND hwnd;
		HTREEITEM tiroot;
		HTREEITEM tisourcefiles;
		HTREEITEM tiheaderfiles;
		HTREEITEM tiresourcefiles;
		HBITMAP bitmapicon[NUM_TREEICONS];
		HIMAGELIST iconlist;
		unsigned long treesourcefiles;
		unsigned long treeheaderfiles;
		unsigned long treeresourcefiles;
		unsigned long treesourcefolders;
		unsigned long treeheaderfolders;
		unsigned long treeresourcefolders;
	public:
		ClassFileTree(void);
		~ClassFileTree(void);
		void SetHwnd(HWND treewnd);
		HWND GetHwnd(void);
		HTREEITEM GetHandle(unsigned char itemtype);
		void ConvertParam(unsigned long lparam, unsigned char &itemtype, unsigned long &id);
		bool GetSelectedItem(unsigned char &itemtype, unsigned long &id);
		bool GetHandleItem(HTREEITEM handle, unsigned char &itemtype, unsigned long &id);
		void InitIcons(HINSTANCE hinst);
		void Clear(void);
		void MakeProjectStructure(char *projectname);
		void Expand(EDGEPROJECT *project);
		void AddSourceFile(char *filename, unsigned long id, HTREEITEM handle = NULL);
		void AddHeaderFile(char *filename, unsigned long id, HTREEITEM handle = NULL);
		void AddResourceFile(char *filename, unsigned long id, HTREEITEM handle = NULL);
		void AddSourceFolder(PRJFOLDER *folder, unsigned long id, HTREEITEM handle = NULL);
		void AddHeaderFolder(PRJFOLDER *folder, unsigned long id, HTREEITEM handle = NULL);
		void AddResourceFolder(PRJFOLDER *folder, unsigned long id, HTREEITEM handle = NULL);
};

/////////////////////////////////////////////////////////////////////
// ClassFileTree: public
/////////////////////////////////////////////////////////////////////

//ClassFileTree: constructor
ClassFileTree::ClassFileTree(void)
{
	unsigned char ctr;
	for (ctr = 0; ctr < NUM_TREEICONS; ctr++)
		bitmapicon[ctr] = NULL;
	treesourcefiles = 0;
	treeheaderfiles = 0;
	treeresourcefiles = 0;
	treesourcefolders = 0;
	treeheaderfolders = 0;
	treeresourcefolders = 0;
	iconlist = NULL;
}

//ClassFileTree destructor
ClassFileTree::~ClassFileTree(void)
{
	unsigned char ctr;
	for (ctr = 0; ctr < NUM_TREEICONS; ctr++)
		if (bitmapicon[ctr] != NULL)
		{
			DeleteObject(bitmapicon[ctr]);
			bitmapicon[ctr] = NULL;
		}
	if (iconlist != NULL)
	{
		ImageList_Destroy(iconlist);
		iconlist = NULL;
	}
}

//Set the window handle of the tree
void ClassFileTree::SetHwnd(HWND treewnd)
{
	hwnd = treewnd;
}

//Get the window handle of the tree
HWND ClassFileTree::GetHwnd(void)
{
	return(hwnd);
}

//Get a handle from the tree
HTREEITEM ClassFileTree::GetHandle(unsigned char itemtype)
{
	if (itemtype == TT_ROOT)
		return(tiroot);
	else if (itemtype == TT_SOURCEROOT)
		return(tisourcefiles);
	else if (itemtype == TT_HEADERROOT)
		return(tiheaderfiles);
	else if (itemtype == TT_RESOURCEROOT)
		return(tiresourcefiles);
	return(NULL);
}

//Convert the tree item parameter
void ClassFileTree::ConvertParam(unsigned long lparam, unsigned char &itemtype, unsigned long &id)
{
	itemtype = (unsigned char)(lparam & 0xFF);
	id = lparam >> 8;
}

//Retrieve information about the selected item
bool ClassFileTree::GetSelectedItem(unsigned char &itemtype, unsigned long &id)
{
	HTREEITEM hitem = (HTREEITEM)SendMessage(hwnd, TVM_GETNEXTITEM, TVGN_CARET, 0);
	return(GetHandleItem(hitem, itemtype, id));
}

//Convert a tree item handle parameter
bool ClassFileTree::GetHandleItem(HTREEITEM handle, unsigned char &itemtype, unsigned long &id)
{
	TVITEM tvitem;
	itemtype = TT_NONE;
	id = 0;
	tvitem.mask = TVIF_HANDLE | TVIF_PARAM;
	tvitem.hItem = handle;
	if (TreeView_GetItem(hwnd, &tvitem) == TRUE)
		ConvertParam(tvitem.lParam, itemtype, id);
	if (itemtype == TT_NONE)
		return(false);
	return(true);
}

//Initialize tree icons
void ClassFileTree::InitIcons(HINSTANCE hinst)
{
	int idx1;
	unsigned char ctr;
	bitmapicon[TI_FOLDEROPEN]     = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_FOLDEROPEN));
	bitmapicon[TI_FOLDERCLOSE]    = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_FOLDERCLOSE));
	bitmapicon[TI_RESFOLDEROPEN]  = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_RESFOLDEROPEN));
	bitmapicon[TI_RESFOLDERCLOSE] = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_RESFOLDERCLOSE));
	bitmapicon[TI_SOURCEFILE]     = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_SOURCEFILE));
	bitmapicon[TI_HEADERFILE]     = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_HEADERFILE));
	bitmapicon[TI_RESOURCEFILE]   = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_RESOURCEFILE));
	bitmapicon[TI_PROJECT]        = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_PROJECT));
	bitmapicon[TI_FOLDERROOT]     = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_FOLDERROOT));
	bitmapicon[TI_RESOURCEROOT]   = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_RESOURCEROOT));
	iconlist = ImageList_Create(16, 16, ILC_COLOR16, NUM_TREEICONS, NUM_TREEICONS);
	if (iconlist != NULL)
	{
		for (ctr = 0; ctr < NUM_TREEICONS; ctr++)
			if (bitmapicon[ctr] != NULL)
				idx1 = ImageList_Add(iconlist, bitmapicon[ctr], NULL);
		TreeView_SetImageList(hwnd, iconlist, 0);
	}
}

//Delete all tree items
void ClassFileTree::Clear(void)
{
	TreeView_DeleteAllItems(hwnd);
	treesourcefiles = 0;
	treeheaderfiles = 0;
	treeresourcefiles = 0;
}

//Create a basic project structure with source and header files
void ClassFileTree::MakeProjectStructure(char *projectname)
{
	TVINSERTSTRUCT lpis;
	lpis.hParent = TVI_ROOT;
	lpis.hInsertAfter        = TVI_ROOT;
	lpis.item.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	lpis.item.iImage         = TI_PROJECT;
	lpis.item.iSelectedImage = TI_PROJECT;
	lpis.item.pszText        = projectname;
	lpis.item.lParam         = TT_ROOT;
	TreeView_InsertItem(hwnd, &lpis);
	tiroot = TreeView_GetRoot(hwnd);

	lpis.hParent             = tiroot;
	lpis.hInsertAfter        = TVI_LAST;
	lpis.item.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	lpis.item.iImage         = TI_FOLDERROOT;
	lpis.item.iSelectedImage = TI_FOLDERROOT;
	lpis.item.pszText        = "Source Files";
	lpis.item.lParam         = TT_SOURCEROOT;
	TreeView_InsertItem(hwnd, &lpis);
	lpis.hParent             = tiroot;
	lpis.hInsertAfter        = TVI_LAST;
	lpis.item.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	lpis.item.iImage         = TI_FOLDERROOT;
	lpis.item.iSelectedImage = TI_FOLDERROOT;
	lpis.item.pszText        = "Header Files";
	lpis.item.lParam         = TT_HEADERROOT;
	TreeView_InsertItem(hwnd, &lpis);
	lpis.hParent             = tiroot;
	lpis.hInsertAfter        = TVI_LAST;
	lpis.item.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	lpis.item.iImage         = TI_RESOURCEROOT;
	lpis.item.iSelectedImage = TI_RESOURCEROOT;
	lpis.item.pszText        = "Resource Files";
	lpis.item.lParam         = TT_RESOURCEROOT;
	TreeView_InsertItem(hwnd, &lpis);
	tisourcefiles   = TreeView_GetChild(hwnd, tiroot);
	tiheaderfiles   = TreeView_GetNextSibling(hwnd, tisourcefiles);
	tiresourcefiles = TreeView_GetNextSibling(hwnd, tiheaderfiles);
}

//Expand the file tree to show all files
void ClassFileTree::Expand(EDGEPROJECT *project)
{
	unsigned long ctr;
	if (project->expandtreetype[TT_ROOT])
		TreeView_Expand(hwnd, tiroot, TVE_EXPAND);
	if (project->expandtreetype[TT_SOURCEROOT])
		TreeView_Expand(hwnd, tisourcefiles, TVE_EXPAND);
	for (ctr = 0; ctr < project->numsourcefolders; ctr++)
		if (project->sourcefld[ctr].expanded)
			TreeView_Expand(hwnd, project->sourcefld[ctr].handle, TVE_EXPAND);
	if (project->expandtreetype[TT_HEADERROOT])
		TreeView_Expand(hwnd, tiheaderfiles, TVE_EXPAND);
	for (ctr = 0; ctr < project->numheaderfolders; ctr++)
		if (project->headerfld[ctr].expanded)
			TreeView_Expand(hwnd, project->headerfld[ctr].handle, TVE_EXPAND);
	if (project->expandtreetype[TT_RESOURCEROOT])
		TreeView_Expand(hwnd, tiresourcefiles, TVE_EXPAND);
	for (ctr = 0; ctr < project->numresourcefolders; ctr++)
		if (project->resourcefld[ctr].expanded)
			TreeView_Expand(hwnd, project->resourcefld[ctr].handle, TVE_EXPAND);
}

//Add a source file to the file tree
void ClassFileTree::AddSourceFile(char *filename, unsigned long id, HTREEITEM handle)
{
	TVINSERTSTRUCT lpis;
	if (handle == NULL)
		lpis.hParent = tisourcefiles;
	else
		lpis.hParent = handle;
	lpis.hInsertAfter        = TVI_LAST;
	lpis.item.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	lpis.item.pszText        = filename;
	lpis.item.iImage         = TI_SOURCEFILE;
	lpis.item.iSelectedImage = TI_SOURCEFILE;
	lpis.item.lParam         = TT_SOURCEFILE + (id << 8);
	TreeView_InsertItem(hwnd, &lpis);
	treesourcefiles++;
}

//Add a source file to the file tree
void ClassFileTree::AddHeaderFile(char *filename, unsigned long id, HTREEITEM handle)
{
	TVINSERTSTRUCT lpis;
	if (handle == NULL)
		lpis.hParent = tiheaderfiles;
	else
		lpis.hParent = handle;
	lpis.hInsertAfter        = TVI_LAST;
	lpis.item.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	lpis.item.pszText        = filename;
	lpis.item.iImage         = TI_HEADERFILE;
	lpis.item.iSelectedImage = TI_HEADERFILE;
	lpis.item.lParam         = TT_HEADERROOT;
	lpis.item.lParam         = TT_HEADERFILE + (id << 8);
	TreeView_InsertItem(hwnd, &lpis);
	treeheaderfiles++;
}

//Add a resource file to the file tree
void ClassFileTree::AddResourceFile(char *filename, unsigned long id, HTREEITEM handle)
{
	TVINSERTSTRUCT lpis;
	if (handle == NULL)
		lpis.hParent = tiresourcefiles;
	else
		lpis.hParent = handle;
	lpis.hInsertAfter        = TVI_LAST;
	lpis.item.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	lpis.item.pszText        = filename;
	lpis.item.iImage         = TI_RESOURCEFILE;
	lpis.item.iSelectedImage = TI_RESOURCEFILE;
	lpis.item.lParam         = TT_RESOURCEFILE + (id << 8);
	TreeView_InsertItem(hwnd, &lpis);
	treeresourcefiles++;
}

//Add a source folder to the tree
void ClassFileTree::AddSourceFolder(PRJFOLDER *folder, unsigned long id, HTREEITEM handle)
{
	TVINSERTSTRUCT lpis;
	if (handle == NULL)
		lpis.hParent = tisourcefiles;
	else
		lpis.hParent = handle;
	lpis.hInsertAfter        = TVI_LAST;
	lpis.item.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	lpis.item.pszText        = folder->name;
	lpis.item.iImage         = TI_FOLDERCLOSE;
	lpis.item.iSelectedImage = TI_FOLDERCLOSE;
	lpis.item.lParam         = TT_SOURCEFOLDER + (id << 8);
	folder->handle = TreeView_InsertItem(hwnd, &lpis);
	treesourcefolders++;
}

//Add a header folder to the tree
void ClassFileTree::AddHeaderFolder(PRJFOLDER *folder, unsigned long id, HTREEITEM handle)
{
	TVINSERTSTRUCT lpis;
	if (handle == NULL)
		lpis.hParent = tiheaderfiles;
	else
		lpis.hParent = handle;
	lpis.hInsertAfter        = TVI_LAST;
	lpis.item.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	lpis.item.pszText        = folder->name;
	lpis.item.iImage         = TI_FOLDERCLOSE;
	lpis.item.iSelectedImage = TI_FOLDERCLOSE;
	lpis.item.lParam         = TT_HEADERFOLDER + (id << 8);
	folder->handle = TreeView_InsertItem(hwnd, &lpis);
	treeheaderfolders++;
}

//Add a resource folder to the tree
void ClassFileTree::AddResourceFolder(PRJFOLDER *folder, unsigned long id, HTREEITEM handle)
{
	TVINSERTSTRUCT lpis;
	if (handle == NULL)
		lpis.hParent = tiresourcefiles;
	else
		lpis.hParent = handle;
	lpis.hInsertAfter        = TVI_LAST;
	lpis.item.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	lpis.item.pszText        = folder->name;
	lpis.item.iImage         = TI_RESFOLDERCLOSE;
	lpis.item.iSelectedImage = TI_RESFOLDERCLOSE;
	lpis.item.lParam         = TT_RESOURCEFOLDER + (id << 8);
	folder->handle = TreeView_InsertItem(hwnd, &lpis);
	treeresourcefolders++;
}

