//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "rpiSensorNode.h"

void RpiSensorNode::getDataFromRep(const OCRepresentation& rep)
{
	rep.getValue("temperature", temperature);
	rep.getValue("humidity", humidity);
	rep.getValue("light", light);
	rep.getValue("sound", sound);
	
	cout << debugInfo << "temperature: " << to_string(temperature) << endl;
	cout << debugInfo << "humidity: " << to_string(humidity) << endl;
	cout << debugInfo << "light: " << to_string(light) << endl;
	cout << debugInfo << "sound: " << to_string(sound) << endl;
}

