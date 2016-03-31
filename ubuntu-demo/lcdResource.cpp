//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "lcdResource.h"

void LcdResource::put(OCRepresentation& recv_rep)
{
	cout << debugInfo << "in PUT" << endl;
	recv_rep.getValue("str", str);
}

void LcdResource::get()
{
	debugPrint({"in GET"});
	debugPrint({"string: ", str});
	rep.setValue("str", str);
}
