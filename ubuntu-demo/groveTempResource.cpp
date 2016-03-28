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
	debugPrint({"in GET"});
	debugPrint({"temperature: ", to_string(temperature)});
	rep.setValue("temperature", temperature);
}

