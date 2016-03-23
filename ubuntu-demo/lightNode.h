//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef LIGHTNODE_H
#define LIGHTNODE_H

#include "sensorNode.h"

class LightNode : public SensorNode
{
public:
	LightNode(string _requestName, string _requestUri) 
		: SensorNode(_requestName, _requestUri) 
	{
		light = 0;
	};

	void getDataFromRep(const OCRepresentation& rep);
	int light;
};

#endif
