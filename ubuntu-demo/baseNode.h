//******************************************************************
// 
// Demo project for Ubuntu Core
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
	BaseNode(string _requestName, ostringstream _requestUri);

	void findResource();
	bool found();

	void get();
	void put();

	bool virtual getEnable() = 0;
	bool virtual putEnable() = 0;

	void virtual getDataFromRep(const OCRepresentation& rep);
	void virtual putDataToRep(OCRepresentation& rep);
	void virtual onPutCheck(OCRepresentation rep);

protected:
	ostringstream requestUri;
	string requestName;
	shared_ptr<OCResource> resourceHandle;
	string hostAddress;

	string debugInfo;

	mutex resourceLock;
	FindCallback fcb;
	// Callback handler on found resource
	void foundResource(shared_ptr<OCResource> resource);

	GetCallback gcb;
	// Callback handler on GET request
	void onGet(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode);

	PutCallback pcb;
	// Callback handler on PUT request
	void onPut(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode);
};

#endif
