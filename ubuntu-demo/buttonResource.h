//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef BUTTONRESOURCE_H
#define BUTTONRESOURCE_H

#include "sensorResource.h"

class ButtonResource : public SensorResource
{
public:
	ButtonResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure) 
		: SensorResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure) 
	{
		button = 0;
		buttonPrev = 0;
	};

	void get();
	bool observeNeedNotification();
	int button;
	int buttonPrev;
};

#endif
