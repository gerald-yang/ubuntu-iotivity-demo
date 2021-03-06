//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "grovepiLedResource.h"


void GrovepiLedResource::get()
{
	debugPrint({"in GET"});

	rep.setValue("red", red);
	rep.setValue("green", green);
	rep.setValue("blue", blue);
}

void GrovepiLedResource::put(OCRepresentation& recv_rep)
{
	debugPrint({"in PUT"});

	recv_rep.getValue("red", red);
	recv_rep.getValue("green", green);
	recv_rep.getValue("blue", blue);

	debugPrint({"Red LED: ", to_string(red)});
	debugPrint({"Green LED: ", to_string(green)});
	debugPrint({"Blue LED: ", to_string(blue)});

	server_write("led_write_red", red);
	server_write("led_write_green", green);
	server_write("led_write_blue", blue);
}

bool GrovepiLedResource::observeNeedNotification()
{
	cout << debugInfo << "Un-supported request OBSERVE" << endl;
	return false;
}
