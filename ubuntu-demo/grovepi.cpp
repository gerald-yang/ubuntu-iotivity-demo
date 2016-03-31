//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "grovepi.h"


Grovepi::Grovepi(int _port)
{
	port = _port;
	debugEnable = true;
	debugInfo = "grovepi> ";

	writeThread = new thread(&Grovepi::writeDataThread, this);
}

Grovepi::~Grovepi()
{
	delete writeThread;
}

void Grovepi::debugPrint(initializer_list<string> list)
{
	if(debugEnable) {
		cout << debugInfo;
		for(string str : list) {
			cout << str;
		}
		cout << endl;
	}
}
void Grovepi::writeDB(string db, string name, double value)
{
	pushDB(db, name, to_string(value));
}

void Grovepi::writeDB(string db, string name, int value)
{
	pushDB(db, name, to_string(value));
}

void Grovepi::pushDB(string db, string name, string value)
{
	struct writeData data;
	data.db = db;
	data.name = name;
	data.value = value;

	lock_guard<mutex> lock(queueLock);
	writeQueue.push(data);
}

void Grovepi::curlWriteDB(string db, string name, string value)
{
}

void Grovepi::writeDataThread()
{
	while(true) {
		if(!writeQueue.empty()) {
			struct writeData data;

			queueLock.lock();
			data = writeQueue.front();
			curlWriteDB(data.db, data.name, data.value);
			writeQueue.pop();
			queueLock.unlock();
		}

		this_thread::sleep_for(chrono::milliseconds(100));
	}
}

