//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "rpiLedResource.h"

void RpiLedResource::get()
{
	debugPrint({"in GET"});

	rep.setValue("red", red);
	rep.setValue("green", green);
	rep.setValue("blue", blue);
}

void RpiLedResource::put(OCRepresentation& recv_rep)
{
	debugPrint({"in PUT"});

	recv_rep.getValue("red", red);
	recv_rep.getValue("green", green);
	recv_rep.getValue("blue", blue);

	return;
}

bool RpiLedResource::observeNeedNotification()
{
	cout << debugInfo << "Un-supported request OBSERVE" << endl;
	return false;
}

