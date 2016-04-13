//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "baseResource.h"


BaseResource::BaseResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure)
{
	resourceHandle = NULL;

	uniqueID = _uniqueID;
	resourceName = _resourceName;
	resourceUri = _resourceUri + uniqueID;
	resourceTypeName = _resourceTypeName;
	isSecure = _isSecure;

	observeThreadStart = false;

	rep.setUri(resourceUri.c_str());

	debugEnable = true;
	debugInfo = resourceName + "> ";
}

void BaseResource::debugPrint(initializer_list<string> list)
{
	if(debugEnable) {
		cout << debugInfo;
		for(string str : list) {
			cout << str;
		}
		cout << endl;
	}
}

void BaseResource::createResource()
{
	uint8_t resourceProperty;

	if(isSecure) {
		resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE;
	} else {
		resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
	}
        cb = bind(&BaseResource::entityHandler, this, PH::_1);

	// This will internally create and register the resource.
	OCStackResult result = OCPlatform::registerResource(
		resourceHandle, resourceUri, resourceTypeName,
		DEFAULT_INTERFACE, cb, resourceProperty);

	if (OC_STACK_OK != result) {
		cout << debugInfo << "Resource creation was unsuccessful" << endl;
	}
}

bool BaseResource::isCreated()
{
	if(resourceHandle)
		return true;
	else
		return false;
}

void BaseResource::observeFunc()
{
	while(true) {
		if(observeNeedNotification()) {

			debugPrint({"Notifying observers"});

			OCStackResult result = OC_STACK_OK;

			result = OCPlatform::notifyAllObservers(resourceHandle);

			if(OC_STACK_NO_OBSERVERS == result) {
				cout << debugInfo << "No More observers, stopping notifications" << endl;
				observeThreadStart = false;
				break;
			}
		}
	}
}

OCEntityHandlerResult BaseResource::entityHandler(shared_ptr<OCResourceRequest> request)
{
	debugPrint({"in entityHandler"});
	OCEntityHandlerResult ehResult = OC_EH_ERROR;

	if(request) {
		// Get the request type and request flag
		string requestType = request->getRequestType();
		int requestFlag = request->getRequestHandlerFlag();

		if(requestFlag & RequestHandlerFlag::RequestFlag) {
			debugPrint({"requestFlag : Request"});
			auto pResponse = make_shared<OC::OCResourceResponse>();
			pResponse->setRequestHandle(request->getRequestHandle());
			pResponse->setResourceHandle(request->getResourceHandle());

			// Check for query params (if any)
			QueryParamsMap queries = request->getQueryParameters();

			if (!queries.empty()) {
				debugPrint({"Query processing upto entityHandler"});
			}
			for (auto it : queries) {
				debugPrint({"Query key: ", it.first, " value : ", it.second});
			}

			if(requestType == "GET") {
				debugPrint({"requestType : GET"});
				get();
				pResponse->setErrorCode(200);
				pResponse->setResponseResult(OC_EH_OK);
				pResponse->setResourceRepresentation(rep);
				if(OC_STACK_OK == OCPlatform::sendResponse(pResponse)) {
					ehResult = OC_EH_OK;
				}
			} else if(requestType == "PUT") {
				debugPrint({"requestType : PUT"});
				OCRepresentation rep = request->getResourceRepresentation();

				put(rep);
				pResponse->setErrorCode(200);
				pResponse->setResponseResult(OC_EH_OK);
				pResponse->setResourceRepresentation(rep);
				if(OC_STACK_OK == OCPlatform::sendResponse(pResponse))
				{
					ehResult = OC_EH_OK;
				}
			} else if(requestType == "POST") {
				debugPrint({"requestType : POST"});
				debugPrint({"unsupported requestType"});
			} else if(requestType == "DELETE") {
				debugPrint({"Delete request received"});
			}
		}

		if(requestFlag & RequestHandlerFlag::ObserverFlag)
		{
			ObservationInfo observationInfo = request->getObservationInfo();
			if(ObserveAction::ObserveRegister == observationInfo.action)
			{
				observers.push_back(observationInfo.obsId);
			}
			else if(ObserveAction::ObserveUnregister == observationInfo.action)
			{
				observers.erase(remove(
					observers.begin(),
					observers.end(),
					observationInfo.obsId),
					observers.end());
			}

			cout << debugInfo << "requestFlag : Observer" << endl;

			// Observation happens on a different thread function.
			// If we have not created the thread already, we will create one here.
			if(!observeThreadStart) {
				observeThread = new thread(&BaseResource::observeFunc, this);
				observeThreadStart = true;
			}

			ehResult = OC_EH_OK;
		}
	} else {
		cout << debugInfo << "Request invalid" << endl;
	}

	return ehResult;
}


