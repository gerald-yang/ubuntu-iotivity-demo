//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef RPISENSORNODE_H
#define RPISENSORNODE_H

#include "sensorNode.h"

class RpiSensorNode : public SensorNode
{
public:
	RpiSensorNode(string _requestName, string _requestUri) 
		: SensorNode(_requestName, _requestUri) 
	{
		temperature = 0.0;
		humidity = 0.0;
		light = 0;
		sound = 0;
	};

	void getDataFromRep(const OCRepresentation& rep);
	double temperature;
	double humidity;
	int light;
	int sound;
};

#endif
