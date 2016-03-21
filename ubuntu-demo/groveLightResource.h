//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVELIGHTRESOURCE_H
#define GROVELIGHTRESOURCE_H

#include "sensorResource.h"

class GroveLightResource : public SensorResource
{
public:
	GroveLightResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure) 
		: SensorResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure) {};

	void get();
	int light;
};

#endif
