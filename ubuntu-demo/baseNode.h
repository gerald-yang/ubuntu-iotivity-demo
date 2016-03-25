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
#include <thread>
#include <chrono>
#include <condition_variable>

#include "OCPlatform.h"
#include "OCApi.h"

using namespace OC;
using namespace std;
namespace PH = std::placeholders;

class BaseNode
{
public:
	BaseNode(string _requestName, string _requestUri);

	void startFindResource();
	bool found();

	void get(bool wait);
	void put(bool wait);
	void observe(bool wait, bool enable);

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

	thread *findThread;
	void findResource();
	mutex resourceLock;
	FindCallback fcb;
	// Callback handler on found resource
	void foundResource(shared_ptr<OCResource> resource);

	mutex getLock;
	bool getDone;
	void setGetDone(bool status);
	void getGetDone(bool status);
	GetCallback gcb;
	// Callback handler on GET request
	void onGet(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode);

	mutex putLock;
	bool putDone;
	void setPutDone(bool status);
	void getPutDone(bool status);
	PutCallback pcb;
	// Callback handler on PUT request
	void onPut(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode);

	bool inObserve;
	ObserveCallback ocb;
	void onObserve(const HeaderOptions /*headerOptions*/, const OCRepresentation& rep, 
			const int& eCode, const int& sequenceNumber);
	ObserveType observe_type;
};

#endif
