//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "ledResource.h"

void LedResource::put(OCRepresentation& recv_rep)
{
	int temp;
	
	debugPrint({"in PUT"});

	recv_rep.getValue("status", temp);
	if(temp < low || temp > high) {
		cout << debugInfo << "value " << temp << " is not valid" << endl;
		return;
	}

	status = temp;

	return;
}

void LedResource::get()
{
	debugPrint({"in GET"});
	rep.setValue("status", status);
}
