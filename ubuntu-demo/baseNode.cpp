//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "baseNode.h"


BaseNode::BaseNode(string _requestName, string _requestUri)
{
	requestUri << OC_RSRVD_WELL_KNOWN_URI << "?rt=" << _requestUri;
	requestName = _requestName;
	resourceHandle = NULL;

	fcb = bind(&BaseNode::foundResource, this, PH::_1);
	gcb = bind(&BaseNode::onGet, this, PH::_1, PH::_2, PH::_3);
	pcb = bind(&BaseNode::onPut, this, PH::_1, PH::_2, PH::_3);

	debugInfo = requestName + "> ";
}

void BaseNode::findResource()
{
	OCPlatform::findResource("", requestUri.str(), CT_DEFAULT, fcb);
}

bool BaseNode::found()
{
	if(resourceHandle)
		return true;
	else
		return false;
}

void BaseNode::get(bool wait)
{
	if(getEnable()) {
		cout << debugInfo << "send GET request" << endl;
		QueryParamsMap test;

		if(wait) {
			setGetDone(false);
		}

		resourceHandle->get(test, gcb);

		if(wait) {
			getGetDone(true);
		}

	} else {
		cout << debugInfo << "not support GET reqeust" << endl;
	}
}

void BaseNode::put(bool wait)
{
	if(putEnable()) {
		cout << debugInfo << "send PUT request" << endl;
		OCRepresentation rep;
		putDataToRep(rep);

		if(wait) {
			setPutDone(false);
		}

		resourceHandle->put(rep, QueryParamsMap(), pcb);

		if(wait) {
			getPutDone(true);
		}
	} else {
		cout << debugInfo << "not support PUT request" << endl;
	}
}

void BaseNode::setGetDone(bool status)
{
	getLock.lock();
	getDone = status;
	getLock.unlock();
}

void BaseNode::getGetDone(bool status)
{
	while(getDone != status) {
		this_thread::sleep_for(chrono::milliseconds(50));
	}
}

void BaseNode::setPutDone(bool status)
{
	putLock.lock();
	putDone = status;
	putLock.unlock();
}

void BaseNode::getPutDone(bool status)
{
	while(putDone != status) {
		this_thread::sleep_for(chrono::milliseconds(50));
	}
}

void BaseNode::getDataFromRep(const OCRepresentation& rep)
{
}

void BaseNode::putDataToRep(OCRepresentation& rep)
{
}

void BaseNode::onPutCheck(OCRepresentation rep)
{
}

// Callback to found resources
void BaseNode::foundResource(shared_ptr<OCResource> resource)
{
	cout << debugInfo << "In foundResource" << endl;

	try {
		lock_guard<mutex> lock(resourceLock);

		if(resourceHandle) {
			if(resourceHandle->uniqueIdentifier() == resource->uniqueIdentifier()) {
				cout << "Found resource " << resource->uniqueIdentifier() << " again, skip" << endl;
			} else {
				cout << "Found another resource " << resource->uniqueIdentifier() << 
					", need to create a new resource node for it" << endl;
			}
		} else {
			resourceHandle = resource;
			hostAddress = resourceHandle->host();

			cout << debugInfo << "Found resource " << resource->uniqueIdentifier();
			cout << " from " << hostAddress << endl;
			cout << debugInfo << "for the first time on server with ID: " << resource->sid() << endl;
		}

	} catch(exception& e) {
		cerr << "Exception in foundResource: "<< e.what() << endl;
	}
}

// Callback to GET request
void BaseNode::onGet(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode)
{
	try {
		if(eCode == OC_STACK_OK) {
			cout << debugInfo << "GET request was successful" << endl;
			getDataFromRep(rep);
		} else {
			cout << "onGET Response error: " << eCode << endl;
			exit(-1);
		}
	} catch(exception& e) {
		cout << "Exception: " << e.what() << " in onGet" << endl;
	}

	setGetDone(true);
}

// Callback after PUT done
void BaseNode::onPut(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode)
{
	try {
		if(eCode == OC_STACK_OK) {
			cout << debugInfo << "PUT request was successful" << endl;
			onPutCheck(rep);
		} else {
			cout << debugInfo << "onPut Response error: " << eCode << endl;
			exit(-1);
		}
	}
	catch(exception& e) {
		cout << debugInfo << "Exception: " << e.what() << " in onPut" << endl;
	}
}

