// code/module.h
// Contains the module base class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

class ClassModule
{
	protected:
		SHARED_DATA *shared;
		unsigned long modulestate;
	public:
		ClassModule(void);
		virtual ~ClassModule(void);
		void LinkShared(SHARED_DATA *pshared);
		void virtual OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta);
		void virtual OnMinimize(void);
		void virtual OnRestore(void);
};


/////////////////////////////////////////////////////////////////////
// ClassModule::Virtual methods                                    //
/////////////////////////////////////////////////////////////////////

void ClassModule::OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta){ }
void ClassModule::OnMinimize(void){ }
void ClassModule::OnRestore(void) { }
ClassModule::~ClassModule(void)   { }

/////////////////////////////////////////////////////////////////////
// ClassModule::Public                                             //
/////////////////////////////////////////////////////////////////////

//ClassModule: constructor
ClassModule::ClassModule(void){ }

//Link the main shared data structure
void ClassModule::LinkShared(SHARED_DATA *pshared)
{
	shared = pshared;
}
