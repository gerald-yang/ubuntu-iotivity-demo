//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef RPISENSORRESOURCE_H
#define RPISENSORRESOURCE_H

#include "baseResource.h"

class RpiSensorResource : public BaseResource
{
public:
	RpiSensorResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure) 
		: BaseResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure)
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
