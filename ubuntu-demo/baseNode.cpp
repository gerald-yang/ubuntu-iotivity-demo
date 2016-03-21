//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "baseNode.h"


BaseNode::BaseNode(string _requestName, ostringstream _requestUri)
{
	requestUri << OC_RSRVD_WELL_KNOWN_URI << "?rt=" << _requestUri.str();
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

void BaseNode::onPut(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode)
{
	try {
		if(eCode == OC_STACK_OK) {
			cout << debugInfo << "PUT request was successful" << endl;
		} else {
			cout << debugInfo << "onPut Response error: " << eCode << endl;
			exit(-1);
		}
	}
	catch(exception& e) {
		cout << debugInfo << "Exception: " << e.what() << " in onPut" << endl;
	}
}

