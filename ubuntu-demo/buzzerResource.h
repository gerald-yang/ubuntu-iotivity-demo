//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef BUZZERRESOURCE_H
#define BUZZERRESOURCE_H

#include "baseResource.h"

class BuzzerResource : public BaseResource
{
public:
	BuzzerResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure) 
		: BaseResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure) 
	{
		tone = 0;
	};

	void get();
	void put(OCRepresentation& recv_rep);
	int tone;
};

#endif
