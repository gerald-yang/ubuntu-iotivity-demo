//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "groveLightResource.h"


void GroveLightResource::get()
{
	light = 0;

	cout << debugInfo << "in GET" << endl;
	rep.setValue("light", light);
	return;
}

