//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "grovepiLcdResource.h"


void GrovepiLcdResource::get()
{
	cout << debugInfo << "Un-supported request GET" << endl;
}

void GrovepiLcdResource::put(OCRepresentation& recv_rep)
{
	debugPrint({"in PUT"});

	recv_rep.getValue("lcd", lcd);

	debugPrint({"LCD string: ", lcd});

	server_write("lcd_write", lcd.c_str());
}

bool GrovepiLcdResource::observeNeedNotification()
{
	cout << debugInfo << "Un-supported request OBSERVE" << endl;
	return false;
}
