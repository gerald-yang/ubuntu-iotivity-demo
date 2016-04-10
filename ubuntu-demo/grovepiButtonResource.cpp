//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "grovepiButtonResource.h"


void GrovepiButtonResource::get()
{
	debugPrint({"in GET"});
	debugPrint({"button: ", to_string(button)});

	server_read("button_read", button);

	rep.setValue("button", button);
}

void GrovepiButtonResource::put(OCRepresentation& recv_rep)
{
	string host = recv_rep.getHost();
	cout << debugInfo << "from " << host << ", Un-supported request PUT" << endl;
}

bool GrovepiButtonResource::observeNeedNotification(void)
{
	server_read("button_read", button);

	if(button != buttonPrev) {
		buttonPrev = button;
		return true;
	} else {
		return false;
	}
}
