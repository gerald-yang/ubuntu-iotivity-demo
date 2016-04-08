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
	debugPrint({"humidity: ", to_string(humidity)});
	debugPrint({"light: ", to_string(light)});
	debugPrint({"sound: ", to_string(sound)});

	server_read("sensor_temp", temperature);
	server_read("sensor_humidity", humidity);
	server_read("sensor_light", light);
	server_read("sensor_sound", sound);

	rep.setValue("temperature", temperature);
	rep.setValue("humidity", humidity);
	rep.setValue("light", light);
	rep.setValue("sound", sound);
}

void GrovepiSensorResource::put(OCRepresentation& recv_rep)
{
	cout << debugInfo << "Un-support request PUT" << endl;
}
