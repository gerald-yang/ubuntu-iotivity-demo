//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVEULTRASONICRESOURCE_H
#define GROVEULTRASONICRESOURCE_H

#include "sensorResource.h"

class GroveUltrasonicResource : public SensorResource
{
public:
	GroveUltrasonicResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure) 
		: SensorResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure) 
	{
		ultrasonic = 0;
	};

	void get();
	int ultrasonic;
};

#endif
