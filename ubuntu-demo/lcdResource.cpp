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
	recv_rep.getValue("str", str);


	cout << debugInfo << "in PUT" << endl;
	return;
}

void LcdResource::get()
{
	debugPrint({"in GET"});
	debugPrint({"string: ", str});
	rep.setValue("str", str);
}
