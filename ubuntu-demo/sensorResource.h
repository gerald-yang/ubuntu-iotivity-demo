//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef SENSORRESOURCE_H
#define SENSORRESOURCE_H

#include "baseResource.h"

class SensorResource : public BaseResource
{
public:
	SensorResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure) 
		: BaseResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure) {};

	virtual void get() = 0;
	void put(OCRepresentation& recv_rep);
	bool observeNeedNotification();
};

#endif
