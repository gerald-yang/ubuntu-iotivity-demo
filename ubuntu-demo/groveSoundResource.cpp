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
	debugPrint({"in GET"});
	debugPrint({"sound: ", to_string(sound)});
	rep.setValue("sound", sound);
}

