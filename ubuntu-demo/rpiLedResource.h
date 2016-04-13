//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef RPILEDRESOURCE_H
#define RPILEDRESOURCE_H

#include "baseResource.h"

class RpiLedResource : public BaseResource
{
public:
	RpiLedResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure) 
		: BaseResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure)
	{
		red = 0;
		green = 0;
		blue = 0;
	};

	void get();
	void put(OCRepresentation& recv_rep);
	bool observeNeedNotification();
	int red;
	int green;
	int blue;
};

#endif
