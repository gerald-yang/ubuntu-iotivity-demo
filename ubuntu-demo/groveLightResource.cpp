//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#include "groveLightResource.h"


void GroveLightResource::get()
{
	light = 0;

	cout << debug_info << "in GET" << endl;
	rep.setValue("light", light);
	return;
}

