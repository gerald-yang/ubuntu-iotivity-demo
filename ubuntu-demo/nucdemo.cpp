#SHIT
//*************************************
//
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//*************************************

#include <pthread.h>
#include <curl/curl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <string.h>

#include <string>
#include <map>
#include <cstdlib>
#include <mutex>
#include <vector>
#include <functional>
#include <condition_variable>
#include <iostream>
#include <fstream>

#include "OCPlatform.h"
#include "OCApi.h"

#include "cpuTempResource.h"
#include "groveTempResource.h"
#include "ledResource.h"


using namespace OC;
using namespace std;

class nuc_node
{
public:
	nuc_node()
	{
		gcb = std::bind(&nuc_node::onGet, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	std::shared_ptr<OCResource> resource;
	string id;
	double cputemp;
	string ipaddr;
	GetCallback gcb;

	// Callback handler on GET request
	void onGet(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode);
	void get();

};

typedef std::map<OCResourceIdentifier, std::shared_ptr<OCResource>> DiscoveredResourceMap;

DiscoveredResourceMap discoveredResources;
std::shared_ptr<OCResource> curResource;
std::mutex resourceLock;
std::mutex vectorLock;
vector<class nuc_node> resourceVector;
string influxdb_ip;
CURL *curl = NULL;

void printResourceInVector()
{
	vector<class nuc_node>::iterator iter;

	cout << "List of resources:" << endl;
	for(iter = resourceVector.begin(); iter != resourceVector.end(); iter++) {
		cout << iter->resource->uri() << endl;
	}
}

bool findResourceInVector(string uri)
{
	vector<class nuc_node>::iterator iter;

	for(iter = resourceVector.begin(); iter != resourceVector.end(); iter++) {
		if(iter->resource->uri() == uri)
			return true;
	}

	return false;
}

void addResourceToVector(std::shared_ptr<OCResource> resource)
{
	std::lock_guard<std::mutex> lock(vectorLock);
	string uri_temp;

	uri_temp = resource->uri();

	nuc_node new_node;
	new_node.resource = resource;
	new_node.id = uri_temp.substr(5);
	new_node.ipaddr = resource->host();

	resourceVector.push_back(new_node);

	//cout << "Add resource: " << endl;
	//cout << "ID: " << new_node.id << endl;
	//cout << "IP: " << new_node.ipaddr << endl;
	//cout << "URI: " << new_node.resource->uri() << endl;
}

void updateCputempInVector(string uri, double cputemp)
{
	vector<class nuc_node>::iterator iter;

	for(iter = resourceVector.begin(); iter != resourceVector.end(); iter++) {
		if(iter->resource->uri() == uri) {
			iter->cputemp = cputemp;
			return;
		}
	}
}

// Callback to found resources
void foundResource(std::shared_ptr<OCResource> resource)
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

int wait_for_network(void)
{
	struct ifaddrs *ifaddr, *ifa;
	int s;
	char host[NI_MAXHOST];

	while(1) {
		if (getifaddrs(&ifaddr) == -1) {
			cout << "getifaddrs failed" << endl;
			return 1;
		}

		for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr == NULL)
				continue;  

			s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

			if(((strncmp(ifa->ifa_name, "eth", 3) == 0)) && (ifa->ifa_addr->sa_family == AF_INET)) {
				if (s != 0) {
					std::cout << "getnameinfo() failed: " << gai_strerror(s) << std::endl;
				} else {
					freeifaddrs(ifaddr);
					return 0;
				}
			}
		}

		freeifaddrs(ifaddr);
		sleep(1);
	}

	return 1;
}

void influxdb_write(string id, double t)
{
	CURLcode res;
	std::string url;
	std::string data;

	url = "http://";
	url += influxdb_ip;
	url += "/write?db=nucdemo";

      
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		data = "temp,nucid=";
		data += id;	
		data += " value=";
		data += std::to_string(t);
		cout << "Write data: " << data << endl;

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK)
			cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
		else
			cout << "curl_easy_perform() succeed" << endl;
	}
}

void * get_temperature (void *param)
{
	vector<class nuc_node>::iterator iter;

	while(1) {
		if(!resourceVector.empty()) {
			for(iter = resourceVector.begin(); iter != resourceVector.end(); iter++) {
				//cout << "Get temperature from: " << iter->resource->uri() << endl;
				iter->get();
			}
		}

		sleep(2);
	}

	return param;
}

void * write_temperature (void *param)
{
	vector<class nuc_node>::iterator iter;

	while(1) {
		if(!resourceVector.empty()) {
			for(iter = resourceVector.begin(); iter != resourceVector.end(); iter++) {
				influxdb_write(iter->id, iter->cputemp);
				usleep(10);
			}
		}

		sleep(2);
	}

	return param;
}

void nuc_node::onGet(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode)
{
	try {
		if(eCode == OC_STACK_OK) {
			double cputemp;
			string uri;

			cout << "GET request was successful" << endl;
			rep.getValue("URI", uri);
			rep.getValue("temperature", cputemp);
			cout << "\ttemperature: " << to_string(cputemp) << endl;

			updateCputempInVector(uri, cputemp);
		} else {
			cout << "onGET Response error: " << eCode << endl;
			exit(-1);
		}
	} catch(std::exception& e) {
		cout << "Exception: " << e.what() << " in onGet" << endl;
	}
}

void nuc_node::get()
{
        QueryParamsMap test;
	//cout << "Resource URI: " << resource->uri() << endl;
	//cout << "Resource ID: " << id << endl;
        resource->get(test, gcb);
}


static FILE* client_open(const char* /*path*/, const char *mode)
{
	return fopen("./oic_svr_db_client.json", mode);
}

int main(int argc, char* argv[]) {

	if(argc == 2) {
		influxdb_ip.assign(argv[1]);
		cout << "InfluxDB IP: " << influxdb_ip << endl;
	}

	if(wait_for_network()) {
		std::cout << "Network is not available" << std::endl;
		return 1;
	}
	
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */ 
	curl = curl_easy_init();
	if(!curl) {
		std::cout << "Can not initiallize curl library" << std::endl;
	}

	std::ostringstream requestURI;
	OCPersistentStorage ps {client_open, fread, fwrite, fclose, unlink };

	// Create PlatformConfig object
	PlatformConfig cfg {
		OC::ServiceType::InProc,
		OC::ModeType::Both,
		"0.0.0.0",
		0,
		OC::QualityOfService::LowQos,
		&ps
	};

	OCPlatform::Configure(cfg);

	// Start a thread to collect all CPU temperature on found resources
	pthread_t threadIdFind;
	pthread_create (&threadIdFind, NULL, get_temperature, NULL);

	// Start a thread to write CPU temperature of all found resources to influxDB
	pthread_t threadIdInfluxdb;
	pthread_create (&threadIdInfluxdb, NULL, write_temperature, NULL);

	resourceVector.clear();

	// Wait for hw-assign to be able to access MAC address in /sys/devices
	ifstream mac_path;
	string mac_addr;

	while(1) {
		//mac_path.open ("/sys/devices/pci0000:00/0000:00:1c.2/0000:03:00.0/net/eth0/address");
		mac_path.open ("/sys/class/net/eth0/address");
		if(mac_path) break;

		cout << "Can not read MAC address, need to use 'snappy hw-assign' to grant permission to this snap" << endl;
		sleep(2);
	}

	getline(mac_path, mac_addr);

	try {
		// Server part
		CpuTempResource cputemp(mac_addr, "CPU temperature", "/nuc/", "nuc.cputemp", false, 
				//"/sys/devices/platform/coretemp.0/hwmon/hwmon0/temp2_input");
				"/sys/devices/platform/coretemp.0/hwmon/hwmon1/device/temp2_input");
        
		cout << "Creating resource ..." << endl;
		cputemp.createResource();
		std::cout << "Created resource" << std::endl;


		// Client part
		// makes it so that all boolean values are printed as 'true/false' in this stream
		cout.setf(ios::boolalpha);
		// Find all resources with nuc.cputemp
		requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=nuc.cputemp";

		cout << "Finding Resource... " << endl;
		while(1) {
			OCPlatform::findResource("", requestURI.str(), CT_DEFAULT, &foundResource);
			sleep(8);
		}

		// A condition variable will free the mutex it is given, then do a non-
		// intensive block until 'notify' is called on it.  In this case, since we
		// don't ever call cv.notify, this should be a non-processor intensive version
		// of while(true);
		std::mutex blocker;
		std::condition_variable cv;
		std::unique_lock<std::mutex> lock(blocker);
		cv.wait(lock);

	} catch(OCException& e) {
		oclog() << "Exception in main: "<<e.what();
	}

	return 0;
}


