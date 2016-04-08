//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVEPILEDRESOURCE_H
#define GROVEPILEDRESOURCE_H

#include "grovepiResource.h"

class GrovepiLedResource : public GrovepiResource
{
public:
	GrovepiLedResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure, int _port) 
		: GrovepiResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure, _port) 
	{
		red = 0;
		green = 0;
		blue = 0;
	};

	void get();
	void put(OCRepresentation& recv_rep);
	int red;
	int green;
	int blue;
};

#endif
