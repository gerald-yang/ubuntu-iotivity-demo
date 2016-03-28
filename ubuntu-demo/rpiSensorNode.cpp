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
	
	debugPrint({"temperature: ", to_string(temperature)});
	debugPrint({"humidity: ", to_string(humidity)});
	debugPrint({"light: ", to_string(light)});
	debugPrint({"sound: ", to_string(sound)});
}

