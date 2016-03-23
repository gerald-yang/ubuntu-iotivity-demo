//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef HUMIDITYNODE_H
#define HUMIDITYNODE_H

#include "sensorNode.h"

class HumidityNode : public SensorNode
{
public:
	HumidityNode(string _requestName, string _requestUri) 
		: SensorNode(_requestName, _requestUri) 
	{
		humidity = 0.0;
	};

	void getDataFromRep(const OCRepresentation& rep);
	double humidity;
};

#endif
