//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "baseNode.h"


BaseNode::BaseNode(string _requestName, string _requestType)
{
	requestType << OC_RSRVD_WELL_KNOWN_URI << "?rt=" << _requestType;
	requestName = _requestName;
	resourceHandle = NULL;

	fcb = bind(&BaseNode::foundResource, this, PH::_1);
	gcb = bind(&BaseNode::onGet, this, PH::_1, PH::_2, PH::_3);
	pcb = bind(&BaseNode::onPut, this, PH::_1, PH::_2, PH::_3);
	ocb = bind(&BaseNode::onObserve, this, PH::_1, PH::_2, PH::_3, PH::_4);
	observe_type = ObserveType::Observe;

	getDone = false;
	putDone = false;
	inObserve = false;

	debugEnable = true;
	debugInfo = requestName + "> ";
}

BaseNode::~BaseNode()
{
	if(inObserve) {
		resourceHandle->cancelObserve();
	}
}

void BaseNode::debugPrint(initializer_list<string> list)
{
	if(debugEnable) {
		cout << debugInfo;
		for(auto str : list) {
			cout << str;
		}
		cout << endl;
	}
}

void BaseNode::startFindResource()
{
	findThread = new thread(&BaseNode::findResource, this);
}

void BaseNode::findResource()
{
	while(resourceHandle == NULL) {
		OCPlatform::findResource("", requestType.str(), CT_DEFAULT, fcb);
		sleep(6);
	}
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
		if(wait) {
			while(!found());
		} else {
			if(!found()) return;
		}

		debugPrint({"send GET request"});
		QueryParamsMap test;

		if(wait) {
			setGetDone(false);
		}

		resourceHandle->get(test, gcb);

		if(wait) {
			getGetDone(true);
		}

	} else {
		cout << debugInfo << "error: not support GET reqeust" << endl;
	}
}

void BaseNode::put(bool wait)
{
	if(putEnable()) {
		if(wait) {
			while(!found());
		} else {
			if(!found()) return;
		}

		debugPrint({"send PUT request"});
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
		cout << debugInfo << "error: not support PUT request" << endl;
	}
}

void BaseNode::observe(bool wait, bool enable)
{
	if(wait) {
		while(!found());
	} else {
		if(!found()) return;
	}

	if(enable && !inObserve) {
		resourceHandle->observe(observe_type, QueryParamsMap(), ocb);
	} else if (!enable && inObserve) {
		resourceHandle->cancelObserve();
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
	debugPrint({"In foundResource"});

	try {
		lock_guard<mutex> lock(resourceLock);

		if(resourceHandle) {
			if(resourceHandle->uniqueIdentifier() == resource->uniqueIdentifier()) {
				cout << debugInfo << "Found resource " << resource->uniqueIdentifier() << " again, skip" << endl;
			} else {
				cout << debugInfo << "Found another resource " << resource->uniqueIdentifier() <<
					", need to create a new resource node for it" << endl;
			}
		} else {
			resourceHandle = resource;
			hostAddress = resourceHandle->host();

			cout << debugInfo << "Found resource " << resource->uniqueIdentifier() << " from " << hostAddress << endl;
			cout << debugInfo << "for the first time on server with ID: " << resource->sid() << endl;
		}

	} catch(exception& e) {
		cout << debugInfo << "Exception in foundResource: "<< e.what() << endl;
	}
}

// Callback to GET request
void BaseNode::onGet(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode)
{
	debugPrint({"onGet"});

	try {
		if(eCode == OC_STACK_OK) {
			debugPrint({"GET request was successful"});
			getDataFromRep(rep);
		} else {
			cout << debugInfo << "onGET Response error: " << eCode << endl;
		}
	} catch(exception& e) {
		cout << debugInfo << "Exception: " << e.what() << " in onGet" << endl;
	}

	setGetDone(true);
}

// Callback after PUT done
void BaseNode::onPut(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode)
{
	debugPrint({"onPut"});

	try {
		if(eCode == OC_STACK_OK) {
			debugPrint({"PUT request was successful"});
			onPutCheck(rep);
		} else {
			cout << debugInfo << "onPut Response error: " << eCode << endl;
		}
	}
	catch(exception& e) {
		cout << debugInfo << "Exception: " << e.what() << " in onPut" << endl;
	}

	setPutDone(true);
}

void BaseNode::onObserve(const HeaderOptions /*headerOptions*/, const OCRepresentation& rep, 
			const int& eCode, const int& sequenceNumber)
{
	debugPrint({"onObserve"});

	try {
		if(eCode == OC_STACK_OK && sequenceNumber != OC_OBSERVE_NO_OPTION) {
			if(sequenceNumber == OC_OBSERVE_REGISTER) {
				debugPrint({"Observe registration action is successful"});
				inObserve = true;
			} else if(sequenceNumber == OC_OBSERVE_DEREGISTER) {
				debugPrint({"Observe De-registration action is successful"});
				inObserve = false;
			}

			debugPrint({"OBSERVE RESULT:"});
			debugPrint({"\tSequenceNumber: ", to_string(sequenceNumber)});
			getDataFromRep(rep);
		} else {
			if(sequenceNumber == OC_OBSERVE_NO_OPTION) {
				debugPrint({"Observe registration or de-registration action is failed"});
			} else {
				cout << debugInfo << "onObserve response error: " << eCode << endl;
			}
			inObserve = false;
		}
	}
	catch(exception& e) {
		cout << debugInfo << "Exception: " << e.what() << " in onObserve" << endl;
		inObserve = false;
	}
}
