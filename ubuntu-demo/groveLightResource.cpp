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

	debugPrint({"in GET"});
	debugPrint({"light: ", to_string(light)});
	rep.setValue("light", light);
}

