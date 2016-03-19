//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#include "baseResource.h"


BaseResource::BaseResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure)
{
	uniqueID = _uniqueID;

	resourceName = _resourceName;
	resourceUri = _resourceUri + uniqueID;
	resourceTypeName = _resourceTypeName;
	isSecure = _isSecure;

	rep.setUri(resourceUri.c_str());

	debug_info = resourceName + "> ";
}

void BaseResource::createResource()
{
	uint8_t resourceProperty;

	if(isSecure) {
		resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE;
	} else {
		resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
	}
        cb = std::bind(&BaseResource::entityHandler, this,PH::_1);

	// This will internally create and register the resource.
	OCStackResult result = OCPlatform::registerResource(
		resourceHandle, resourceUri, resourceTypeName,
		DEFAULT_INTERFACE, cb, resourceProperty);

	if (OC_STACK_OK != result) {
		cout << debug_info << "Resource creation was unsuccessful" << endl;
	}
}

OCEntityHandlerResult BaseResource::entityHandler(std::shared_ptr<OCResourceRequest> request)
{
	cout << debug_info << "in entityHandler" << endl;
	OCEntityHandlerResult ehResult = OC_EH_ERROR;

	if(request) {
		// Get the request type and request flag
		string requestType = request->getRequestType();
		int requestFlag = request->getRequestHandlerFlag();

		if(requestFlag & RequestHandlerFlag::RequestFlag) {
			cout << debug_info << "requestFlag : Request" << endl;
			auto pResponse = std::make_shared<OC::OCResourceResponse>();
			pResponse->setRequestHandle(request->getRequestHandle());
			pResponse->setResourceHandle(request->getResourceHandle());

			// Check for query params (if any)
			QueryParamsMap queries = request->getQueryParameters();

			if (!queries.empty()) {
				cout << debug_info << "Query processing upto entityHandler" << endl;
			}
			for (auto it : queries) {
				cout << debug_info << "Query key: " << it.first << " value : " << it.second << endl;
			}

			if(requestType == "GET") {
				cout << debug_info << "requestType : GET" << endl;
				get();
				pResponse->setErrorCode(200);
				pResponse->setResponseResult(OC_EH_OK);
				pResponse->setResourceRepresentation(rep);
				if(OC_STACK_OK == OCPlatform::sendResponse(pResponse)) {
					ehResult = OC_EH_OK;
				}
			} else if(requestType == "PUT") {
				cout << debug_info << "requestType : PUT" << endl;
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
				cout << debug_info << "requestType : POST" << endl;
				cout << debug_info << "unsupported requestType" << endl;
			} else if(requestType == "DELETE") {
				cout << debug_info << "Delete request received" << endl;
			}
		}

#if 0
		if(requestFlag & RequestHandlerFlag::ObserverFlag)
		{
			ObservationInfo observationInfo = request->getObservationInfo();
			if(ObserveAction::ObserveRegister == observationInfo.action)
			{
				m_interestedObservers.push_back(observationInfo.obsId);
			}
			else if(ObserveAction::ObserveUnregister == observationInfo.action)
			{
				m_interestedObservers.erase(std::remove(
					m_interestedObservers.begin(),
					m_interestedObservers.end(),
					observationInfo.obsId),
					m_interestedObservers.end());
			}

			pthread_t threadId;

			cout << "\t\trequestFlag : Observer\n";
			gObservation = 1;
			static int startedThread = 0;

			// Observation happens on a different thread in ChangeLightRepresentation function.
			// If we have not created the thread already, we will create one here.
			if(!startedThread)
			{
				pthread_create (&threadId, NULL, ChangeLightRepresentation, (void *)this);
				startedThread = 1;
			}
			ehResult = OC_EH_OK;
		}
#endif
	} else {
		cout << debug_info << "Request invalid" << endl;
	}

	return ehResult;
}


