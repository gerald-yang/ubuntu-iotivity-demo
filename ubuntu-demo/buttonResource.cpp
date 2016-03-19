//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#include "buttonResource.h"

void ButtonResource::get()
{
	cout << debug_info << "in GET" << endl;
	rep.setValue("status", status);
	return;
}

