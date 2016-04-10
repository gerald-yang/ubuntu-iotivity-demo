//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "grovepiUltrasonicResource.h"


void GrovepiUltrasonicResource::get()
{
	debugPrint({"in GET"});
	debugPrint({"ultrasonic: ", to_string(ultrasonic)});

	server_read("ultrasonic_read", ultrasonic);

	rep.setValue("ultrasonic", ultrasonic);
}

void GrovepiUltrasonicResource::put(OCRepresentation& recv_rep)
{
	string host = recv_rep.getHost();
	cout << debugInfo << "from " << host << ", Un-supported request PUT" << endl;
}

bool GrovepiUltrasonicResource::observeNeedNotification()
{
	cout << debugInfo << "Un-supported request OBSERVE" << endl;
	return false;
}
