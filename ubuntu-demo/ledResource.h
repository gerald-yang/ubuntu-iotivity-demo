//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef LEDRESOURCE_H
#define LEDRESOURCE_H

#include "baseResource.h"

class LedResource : public BaseResource
{
public:
	LedResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure, int _low, int _high) 
		: BaseResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure)
	{
		low = _low;
		high = _high;
		status = 0;
	};

	void get();
	void put(OCRepresentation& recv_rep);
	int low;
	int high;
	int status;
};

#endif
