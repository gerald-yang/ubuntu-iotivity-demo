//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef SENSORNODE_H
#define SENSORNODE_H

#include "baseNode.h"

class SensorNode : public BaseNode
{
public:
	SensorNode(string _requestName, string _requestUri) 
		: BaseNode(_requestName, _requestUri) 
	{
	};

	bool getEnable();
	bool putEnable();
	virtual void getDataFromRep(const OCRepresentation& rep) = 0;
};

#endif
