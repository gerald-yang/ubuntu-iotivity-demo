//******************************************************************
// 
// Demo project for Intel platform
//
// Author: Gerald Yang
//
//******************************************************************

#include "cpuTempResource.h"

void CpuTempResource::get()
{
	ifstream temp_path;
	string temp;

	temp_path.open(sysfs_path);
	if(!temp_path) {
		cout << "Can not read CPU temperature sensor, probably need to use 'snappy hw-assign' to grant permission to this snap" << endl;
		return;
	}

	getline(temp_path, temp);
	temperature = stod(temp, NULL);
	temperature = temperature / 1000;

	cout << debug_info << "in GET" << endl;
	rep.setValue("URI", resourceUri);
	rep.setValue("temperature", temperature);
	return;
}

