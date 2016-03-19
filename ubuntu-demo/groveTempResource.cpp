//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#include "groveTempResource.h"


void GroveTempResource::get()
{
	cout << debug_info << "in GET" << endl;
	rep.setValue("URI", resourceUri);
	rep.setValue("temperature", temperature);
	return;
}

