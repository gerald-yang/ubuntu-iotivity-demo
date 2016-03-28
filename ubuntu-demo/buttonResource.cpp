//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "buttonResource.h"

void ButtonResource::get()
{
	debugPrint({"in GET"});
	debugPrint({"status: ", to_string(status)});
	rep.setValue("status", status);
}

