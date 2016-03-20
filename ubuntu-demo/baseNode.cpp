//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#include "baseNode.h"


BaseNode::BaseNode(string _requestName, string _requestUri)
{
	requestUri << OC_RSRVD_WELL_KNOWN_URI << "?rt=" << _requestUri;
	requestName = _requestName;

	fcb = std::bind(&BaseNode::foundResource, this, std::placeholders::_1);

	debug_info = requestName + "> ";
}

void BaseNode::findResource()
{
	OCPlatform::findResource("", requestUri.str(), CT_DEFAULT, &foundResource);
}

// Callback to found resources
void BaseNode::foundResource(std::shared_ptr<OCResource> resource)
{
	std::cout << "In foundResource\n";
	std::string resourceURI;
	std::string hostAddress;

	try {
		std::lock_guard<std::mutex> lock(resourceLock);

		if(discoveredResources.find(resource->uniqueIdentifier()) == discoveredResources.end()) {
			cout << "Found resource " << resource->uniqueIdentifier() <<
				" for the first time on server with ID: " << resource->sid() << endl;
			discoveredResources[resource->uniqueIdentifier()] = resource;

			if(!findResourceInVector(resource->uri())) {
				addResourceToVector(resource);
				printResourceInVector();
			} else {
				cout << "URI: " << resource->uri() << " already in vector, skip" << endl;
			}
		} else {
			cout << "Found resource " << resource->uniqueIdentifier() << " again, skip" << endl;
		}

	} catch(std::exception& e) {
		std::cerr << "Exception in foundResource: "<< e.what() << std::endl;
	}
}


