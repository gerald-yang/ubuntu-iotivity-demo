//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVEPISENSORRESOURCE_H
#define GROVEPISENSORRESOURCE_H

#include "grovepiResource.h"

class GrovepiSensorResource : public GrovepiResource
{
public:
	GrovepiSensorResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure, int _port) 
		: GrovepiResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure, _port) 
	{
		temperature = 0.0;
		humidity = 0.0;
		light = 0;
		sound = 0;
	};

	void get();
	void put(OCRepresentation& recv_rep);
	bool observeNeedNotification();
	double temperature;
	double humidity;
	int light;
	int sound;
};

#endif
