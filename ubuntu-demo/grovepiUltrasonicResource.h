//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVEPIULTRASONICRESOURCE_H
#define GROVEPIULTRASONICRESOURCE_H

#include "grovepiResource.h"

class GrovepiUltrasonicResource : public GrovepiResource
{
public:
	GrovepiUltrasonicResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure, int _port) 
		: GrovepiResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure, _port) 
	{
		ultrasonic = 0;
	};

	void get();
	void put(OCRepresentation& recv_rep);
	int ultrasonic;
};

#endif
