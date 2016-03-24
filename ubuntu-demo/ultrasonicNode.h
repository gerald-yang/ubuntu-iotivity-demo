//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef ULTRASONICNODE_H
#define ULTRASONICNODE_H

#include "sensorNode.h"

class UltrasonicNode : public SensorNode
{
public:
	UltrasonicNode(string _requestName, string _requestUri) 
		: SensorNode(_requestName, _requestUri) 
	{
		ultrasonic = 0;
	};

	void getDataFromRep(const OCRepresentation& rep);
	int ultrasonic;
};

#endif
