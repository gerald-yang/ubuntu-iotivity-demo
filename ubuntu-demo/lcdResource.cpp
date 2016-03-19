//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#include "lcdResource.h"

void LcdResource::put(OCRepresentation& recv_rep)
{
	recv_rep.getValue("str", str);


	cout << debug_info << "in PUT" << endl;
	return;
}

void LcdResource::get()
{
	rep.setValue("str", str);

	cout << debug_info << "in GET" << endl;
	return;
}
