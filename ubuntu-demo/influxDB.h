//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef INFLUXDB_H
#define INFLUXDB_H


#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <chrono>

#include <unistd.h>

#include <curl/curl.h>

using namespace std;

class InfluxDB
{
public:
	InfluxDB(string _address);
	~InfluxDB();
	void writeDB(string db, string name, double value);
	void writeDB(string db, string name, int value);

	void debugPrint(initializer_list<string> list);
protected:
	string address;
	CURL *curl;

	struct writeData {
		string db;
		string name;
		string value;
	};

	mutex queueLock;
	queue<struct writeData> writeQueue;

	bool debugEnable;
	string debugInfo;

	void pushDB(string db, string name, string value);
	void curlWriteDB(string db, string name, string value);
	thread *writeThread;
	void writeDataThread();
};

#endif
