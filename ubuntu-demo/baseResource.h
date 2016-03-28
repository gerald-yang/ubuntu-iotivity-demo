//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef BASERESOURCE_H
#define BASERESOURCE_H


#include <functional>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <mutex>
#include <condition_variable>

#include <pthread.h>

#include "OCPlatform.h"
#include "OCApi.h"

using namespace std;
using namespace OC;
namespace PH = std::placeholders;

class BaseResource
{
public:
	BaseResource(string _resourceName, string _uniqueID, string _resourceUri, string _resourceTypeName, bool _isSecure);

	void createResource();
	virtual void put(OCRepresentation&) = 0;
	virtual void get() = 0;

	void debugPrint(initializer_list<string> list);

protected:
	string uniqueID;

	string resourceName;
	string resourceUri;
	string resourceTypeName;

	bool debugEnable;
	string debugInfo;

	bool isSecure;

	OCResourceHandle resourceHandle;
	OCRepresentation rep;
	ObservationIds observers;

	EntityHandler cb;

	OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> request);
};

#endif
