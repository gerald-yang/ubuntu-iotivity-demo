//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "grovepiSensorResource.h"


void GrovepiSensorResource::get()
{
	debugPrint({"in GET"});
	debugPrint({"temperature: ", to_string(temperature)});
	rep.setValue("temperature", temperature);
}

void GrovepiSensorResource::put(OCRepresentation& recv_rep)
{
	cout << debugInfo << "Un-support request PUT" << endl;
}
