//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "rpiSensorResource.h"

void RpiSensorResource::get()
{
	debugPrint({"in GET"});
	
	rep.setValue("temperature", temperature);
	rep.setValue("humidity", humidity);
	rep.setValue("light", light);
	rep.setValue("sound", sound);
}

void RpiSensorResource::put(OCRepresentation& recv_rep)
{
	debugPrint({"in PUT"});

	rep.getValue("temperature", temperature);
	rep.getValue("humidity", humidity);
	rep.getValue("light", light);
	rep.getValue("sound", sound);
	
	debugPrint({"temperature: ", to_string(temperature)});
	debugPrint({"humidity: ", to_string(humidity)});
	debugPrint({"light: ", to_string(light)});
	debugPrint({"sound: ", to_string(sound)});

	return;
}

bool RpiSensorResource::observeNeedNotification()
{
	cout << debugInfo << "Un-supported request OBSERVE" << endl;
	return false;
}

