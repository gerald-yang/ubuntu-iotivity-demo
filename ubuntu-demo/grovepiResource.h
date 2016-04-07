//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVEPIRESOURCE_H
#define GROVEPIRESOURCE_H


#include <iostream>

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>

#include "baseResource.h"

class GrovepiResource : public BaseResource
{
public:
	GrovepiResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure, int _port) 
		: BaseResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure)
	{
		port = _port;
	}

	virtual void get() = 0;
	virtual void put(OCRepresentation& recv_rep) = 0;

	int server_connect();
	int server_read(const char *name, double& value);
	int server_read(const char *name, int& value);
	int server_write(const char *name, double value);
	int server_write(const char *name, int value);
	int server_write(const char *name, const char *str);

protected:

	int port;
	struct sockaddr_in serv_addr;
	int sockfd;
	static const int packetLen = 64;
};

#endif
