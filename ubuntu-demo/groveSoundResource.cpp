//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "groveSoundResource.h"


void GroveSoundResource::get()
{
	cout << debugInfo << "in GET" << endl;
	rep.setValue("sound", sound);
	return;
}

