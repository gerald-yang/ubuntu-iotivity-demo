//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "lcdResource.h"

void LcdResource::put(OCRepresentation& recv_rep)
{
	cout << debugInfo << "in PUT" << endl;
	if(str.size() >= (unsigned int)(row * column)) {
		cout << debugInfo << "Error: LCD string size " << to_string(str.size()) << " out of range " << str << endl;
	} else {
		recv_rep.getValue("str", str);
	}
}

void LcdResource::get()
{
	debugPrint({"in GET"});
	debugPrint({"string: ", str});
	rep.setValue("str", str);
}

bool LcdResource::observeNeedNotification()
{
	cout << debugInfo << "Un-supported request OBSERVE" << endl;
	return false;
}

