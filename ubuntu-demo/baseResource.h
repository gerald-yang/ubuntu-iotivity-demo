//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef BASERESOURCE_H
#define BASERESOURCE_H


#include <functional>
#include <iostream>
#include <fstream>
#include <mutex>
#include <condition_variable>

#include <pthread.h>

#include "OCPlatform.h"
#include "OCApi.h"

using namespace OC;
using namespace std;
namespace PH = std::placeholders;

class BaseResource
{
public:
	BaseResource(string _resourceName, string _uniqueID, string _resourceUri, string _resourceTypeName, bool _isSecure);

	void createResource();
	virtual void put(OCRepresentation&) = 0;
	virtual void get() = 0;

protected:
	string uniqueID;

	string resourceName;
	string resourceUri;
	string resourceTypeName;

	string debug_info;

	bool isSecure;

	OCResourceHandle resourceHandle;
	OCRepresentation rep;
	ObservationIds observers;

	EntityHandler cb;

	OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> request);
};

#endif
