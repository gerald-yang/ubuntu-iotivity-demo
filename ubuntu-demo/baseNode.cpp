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

bool BaseNode::found()
{
	if(resourceHandle)
		return true;
	else
		return false;
}

void BaseNode::findResource()
{
	OCPlatform::findResource("", requestUri.str(), CT_DEFAULT, fcb);
}

void BaseNode::get()
{
	if(getEnable()) {
		cout << debugInfo << "send GET request" << endl;
		QueryParamsMap test;
		resourceHandle->get(test, gcb);
	} else {
		cout << debugInfo << "not support GET reqeust" << endl;
	}
}

void BaseNode::put()
{
	if(putEnable()) {
		cout << debugInfo << "send PUT request" << endl;
		OCRepresentation rep;
		putDataToRep(rep);
		resourceHandle->put(rep, QueryParamsMap(), pcb);
	} else {
		cout << debugInfo << "not support PUT request" << endl;
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
			cout << "Found resource " << resource->uniqueIdentifier() <<
				" for the first time on server with ID: " << resource->sid() << endl;
			resourceHandle = resource;
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

