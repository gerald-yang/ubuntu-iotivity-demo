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
	cout << debugInfo << "from " << host << ", Un-surrpoted request for sensor" << endl;
}

