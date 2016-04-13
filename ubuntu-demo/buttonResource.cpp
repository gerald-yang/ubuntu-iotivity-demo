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
	debugPrint({"button: ", to_string(button)});
	rep.setValue("button", button);
}

bool ButtonResource::observeNeedNotification(void)
{
	if(button != buttonPrev) {
		buttonPrev = button;
		return true;
	} else {
		return false;
	}
}
