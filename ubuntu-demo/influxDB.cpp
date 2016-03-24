//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "influxDB.h"


InfluxDB::InfluxDB(string _address)
{
	address = _address;
	debugInfo = "influxDB> ";

	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */ 
	curl = curl_easy_init();
	if(!curl) {
		cout << debugInfo << "Can not initiallize curl library" << endl;
	}

	writeThread = new thread(&InfluxDB::writeDataThread, this);
}

InfluxDB::~InfluxDB()
{
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	delete writeThread;
}

void InfluxDB::writeDB(string db, string name, double value)
{
	pushDB(db, name, to_string(value));
}

void InfluxDB::writeDB(string db, string name, int value)
{
	pushDB(db, name, to_string(value));
}

void InfluxDB::pushDB(string db, string name, string value)
{
	struct writeData data;
	data.db = db;
	data.name = name;
	data.value = value;

	lock_guard<mutex> lock(queueLock);
	writeQueue.push(data);
}

void InfluxDB::curlWriteDB(string db, string name, string value)
{
	CURLcode res;
	string url;
	string data;

	url = "http://";
	url += address;
	url += "/write?db=";
	url += db;

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		data = name;
		data += ",sensor=1 value=";
		data += value;

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK)
			cout << debugInfo << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
	} else {
		cout << debugInfo << "curl library unavailable" << endl;
	}
}

void InfluxDB::writeDataThread()
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

