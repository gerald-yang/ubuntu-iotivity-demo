//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVEPIBUTTONRESOURCE_H
#define GROVEPIBUTTONRESOURCE_H

#include "grovepiResource.h"

class GrovepiButtonResource : public GrovepiResource
{
public:
	GrovepiButtonResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure, int _port) 
		: GrovepiResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure, _port) 
	{
		button = 0;
		buttonPrev = 0;
	};

	void get();
	void put(OCRepresentation& recv_rep);
	bool observeNeedNotification();
	int button;
	int buttonPrev;
};

#endif
