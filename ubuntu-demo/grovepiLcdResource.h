//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVEPILCDRESOURCE_H
#define GROVEPILCDRESOURCE_H

#include "grovepiResource.h"

class GrovepiLcdResource : public GrovepiResource
{
public:
	GrovepiLcdResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure, int _port) 
		: GrovepiResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure, _port) 
	{
		lcd = "";
	};

	void get();
	void put(OCRepresentation& recv_rep);
	bool observeNeedNotification();
	string lcd;
};

#endif
