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
	cout << debugInfo << "in GET" << endl;
	rep.setValue("status", status);
	return;
}

