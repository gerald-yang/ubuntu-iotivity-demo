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
	recv_rep.getValue("status", temp);
	if(temp < low || temp > high) {
		cout << debugInfo << "value " << temp << " is not valid" << endl;
		return;
	}

	status = temp;

	cout << debugInfo << "in PUT" << endl;
	return;
}

void LedResource::get()
{
	rep.setValue("status", status);

	cout << debugInfo << "in GET" << endl;
	return;
}
