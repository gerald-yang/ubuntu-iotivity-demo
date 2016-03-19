//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVESOUNDRESOURCE_H
#define GROVESOUNDRESOURCE_H

#include "sensorResource.h"

class GroveSoundResource : public SensorResource
{
public:
	GroveSoundResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure) 
		: SensorResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure) 
	{
		sound = 0;
	};

	void get();
	int sound;
};

#endif
