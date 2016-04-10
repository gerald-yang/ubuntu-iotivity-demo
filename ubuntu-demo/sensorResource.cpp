//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "sensorResource.h"

void SensorResource::put(OCRepresentation& recv_rep)
{
	string host = recv_rep.getHost();
	cout << debugInfo << "from " << host << ", Un-supported request PUT for sensor" << endl;
}

bool SensorResource::observeNeedNotification()
{
	cout << debugInfo << "Un-supported request OBSERVE for sensor" << endl;
	return false;
}

