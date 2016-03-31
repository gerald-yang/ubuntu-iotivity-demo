//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVEPI_H
#define GROVEPI_H


#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <chrono>

#include <unistd.h>

using namespace std;

class Grovepi
{
public:
	Grovepi(int _port);
	~Grovepi();
	void writeDB(string db, string name, double value);
	void writeDB(string db, string name, int value);

	void debugPrint(initializer_list<string> list);
protected:
	int port;

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
