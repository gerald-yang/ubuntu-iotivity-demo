//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef SOUNDNODE_H
#define SOUNDNODE_H

#include "sensorNode.h"

class SoundNode : public SensorNode
{
public:
	SoundNode(string _requestName, string _requestUri) 
		: SensorNode(_requestName, _requestUri) 
	{
		sound = 0;
	};

	void getDataFromRep(const OCRepresentation& rep);
	int sound;
};

#endif
