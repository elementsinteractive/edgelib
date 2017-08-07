// netapi.h
// Extended API's related to networking
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED     //Can only be included by the main engine
#ifndef ENETAPI_INCLUDED  //Make sure the API definition file isn't included multiple times
#define ENETAPI_INCLUDED

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

class ClassENetCapture
{
	friend class ClassEConnect;
	protected:
		ClassEConnect *net;
		virtual void OnNetMessage(long id, long msg, long bufsize, unsigned char *buffer) = 0;
	public:
		virtual ~ClassENetCapture(void){ };
};//The network data capture class for derivation

#endif
#endif
