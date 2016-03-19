//******************************************************************
// 
// Demo project for Intel platform
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
		cout << debug_info << "value " << temp << " is not valid" << endl;
		return;
	}

	status = temp;

	cout << debug_info << "in PUT" << endl;
	return;
}

void LedResource::get()
{
	rep.setValue("status", status);

	cout << debug_info << "in GET" << endl;
	return;
}
