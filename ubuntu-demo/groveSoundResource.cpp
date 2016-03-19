//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#include "groveSoundResource.h"


void GroveSoundResource::get()
{
	cout << debug_info << "in GET" << endl;
	rep.setValue("sound", sound);
	return;
}

