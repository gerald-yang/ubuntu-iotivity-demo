//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef BASENODE_H
#define BASENODE_H


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

class BaseNode
{
public:
	BaseNode(string _requestName, string _requestUri);

	void findResource();

protected:
	string requestUri;

	string debug_info;

	FindCallback fcb;
	foundResource(std::shared_ptr<OCResource> resource)

	DiscoveredResourceMap discoveredResources;
	shared_ptr<OCResource> resource;
	mutex resourceLock;
	mutex vectorLock;
};

#endif
