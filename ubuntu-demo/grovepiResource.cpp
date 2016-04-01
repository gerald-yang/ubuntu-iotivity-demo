//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "grovepiResource.h"

int GrovepiResource::server_connect()
{
	int result;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		debugPrint({"ERROR: can not create socket"});
		return -1;
	}

	while(1) {
		result = connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
		if (result < 0) {
			debugPrint({"Retry to connect to python server"});
		} else {
			debugPrint({"Connection has been built"});
			break;
		}

		sleep(2);
	}

	return 0;
}

int GrovepiResource::server_read(const char *name, double& value)
{
	int n;
	char wdata[packetLen] = {0};
	char data[packetLen] = {0};

	sprintf(wdata, "%32s", name);
	//cout << "data: " << wdata << endl;
	n = write(sockfd, wdata, packetLen);

	n = read(sockfd, data, packetLen);
	if(n < 0) {
		debugPrint({"Read from server failed"});
		return -1;
	} else {
		debugPrint({"Read data: ", data});
		value = stod(data);
		return 0;
	}
}

int GrovepiResource::server_read(const char *name, int& value)
{
	int n;
	char wdata[packetLen] = {0};
	char data[packetLen] = {0};

	sprintf(wdata, "%32s", name);
	n = write(sockfd, wdata, packetLen);

	n = read(sockfd, data, packetLen);
	if(n < 0) {
		debugPrint({"Read from server failed"});
		return -1;
	} else {
		debugPrint({data});
		value = stoi(data);
		return 0;
	}
}

int GrovepiResource::server_write(const char *name, double value)
{
	int n;
	char wdata[packetLen] = {0};

	sprintf(wdata, "%32s%d", name, (int)value);

	debugPrint({"data: ", wdata});
	n = write(sockfd, wdata, packetLen);
	if(n < 0) {
		debugPrint({"Write to server failed"});
		return -1;
	} else {
		return 0;
	} 
}

int GrovepiResource::server_write(const char *name, int value)
{
	int n;
	char wdata[packetLen] = {0};

	sprintf(wdata, "%32s%d", name, value);

	debugPrint({"data: ", wdata});
	n = write(sockfd, wdata, packetLen);
	if(n < 0) {
		debugPrint({"Write to server failed"});
		return -1;
	} else {
		return 0;
	} 
}

int GrovepiResource::server_write(const char *name, const char *str)
{
	int n;
	char wdata[packetLen+1] = {0};

	sprintf(wdata, "%32s%-32s", name, str);

	debugPrint({"data: ", wdata});
	n = write(sockfd, wdata, packetLen);
	if(n < 0) {
		debugPrint({"Write to server failed"});
		return -1;
	} else {
		return 0;
	} 
}

