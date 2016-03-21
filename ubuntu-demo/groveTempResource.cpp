//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "groveTempResource.h"


void GroveTempResource::get()
{
	cout << debugInfo << "in GET" << endl;
	rep.setValue("URI", resourceUri);
	rep.setValue("temperature", temperature);
	return;
}

