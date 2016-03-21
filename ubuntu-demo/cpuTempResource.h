//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef CPUTEMPRESOURCE_H
#define CPUTEMPRESOURCE_H

#include "sensorResource.h"

class CpuTempResource : public SensorResource
{
public:
	CpuTempResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure, string _sysfs_path) 
		: SensorResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure) 
	{
		sysfs_path = _sysfs_path;
		temperature = 0.0;
	};

	void get();
	string sysfs_path;
	double temperature;
};

#endif
