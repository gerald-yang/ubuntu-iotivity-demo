//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#include "sensorResource.h"

void SensorResource::put(OCRepresentation& recv_rep)
{
	string host = recv_rep.getHost();
	cout << debug_info << "from " << host << ", Un-surrpoted request for sensor" << endl;
	return;
}

