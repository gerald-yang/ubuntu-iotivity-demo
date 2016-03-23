//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef BASENODE_H
#define BASENODE_H


#include <iostream>
#include <list>
#include <mutex>

#include <curl/curl.h>

using namespace std;

class InfluxDB
{
public:
	InfluxDB(string _address);
	void writeDB(string db, string name, double value);
	void writeDB(string db, string name, int value);

protected:
	string address;
	CURL *curl;
	mutex vectorLock;
	list<struct writeData> writeList;
	struct writeData {
		string db;
		string name;
		string value;
	};


	string debugInfo;

	void _writeDB(string db, string name, string value);
};

#endif
