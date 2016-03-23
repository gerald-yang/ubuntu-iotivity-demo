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
}

void InfluxDB::writeDB(string db, string name, double value)
{
	_writeDB(db, name, to_string(value));
}

void InfluxDB::writeDB(string db, string name, int value)
{
	_writeDB(db, name, to_string(value));
}

void InfluxDB::_writeDB(string db, string name, string value)
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

