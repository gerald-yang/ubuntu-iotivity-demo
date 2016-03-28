//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVETEMPRESOURCE_H
#define GROVETEMPRESOURCE_H

#include "sensorResource.h"

class GroveTempResource : public SensorResource
{
public:
	GroveTempResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure) 
		: SensorResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure) 
	{
		temperature = 0.0;
		debugEnable = false;
	};

	void get();
	double temperature;
};

#endif
