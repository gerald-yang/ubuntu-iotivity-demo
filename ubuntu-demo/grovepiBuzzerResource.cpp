//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "grovepiBuzzerResource.h"


void GrovepiBuzzerResource::get()
{
	cout << debugInfo << "Un-supported request GET" << endl;
}

void GrovepiBuzzerResource::put(OCRepresentation& recv_rep)
{
	debugPrint({"in PUT"});

	recv_rep.getValue("buzzer", buzzer);

	debugPrint({"buzzer: ", to_string(buzzer)});

	server_write("buzzer_write", buzzer);
}

bool GrovepiBuzzerResource::observeNeedNotification()
{
	cout << debugInfo << "Un-supported request OBSERVE" << endl;
	return false;
}
