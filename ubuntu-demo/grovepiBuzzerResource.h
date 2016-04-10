//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVEPIBUZZERRESOURCE_H
#define GROVEPIBUZZERRESOURCE_H

#include "grovepiResource.h"

class GrovepiBuzzerResource : public GrovepiResource
{
public:
	GrovepiBuzzerResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure, int _port) 
		: GrovepiResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure, _port) 
	{
		buzzer = 0;
	};

	void get();
	void put(OCRepresentation& recv_rep);
	bool observeNeedNotification();
	int buzzer;
};

#endif
