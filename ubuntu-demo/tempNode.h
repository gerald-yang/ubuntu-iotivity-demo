//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef TEMPNODE_H
#define TEMPNODE_H

#include "sensorNode.h"

class TempNode : public SensorNode
{
public:
	TempNode(string _requestName, string _requestUri) 
		: SensorNode(_requestName, _requestUri) 
	{
		temperature = 0.0;
	};

	void getDataFromRep(const OCRepresentation& rep);
	double temperature;
};

#endif
