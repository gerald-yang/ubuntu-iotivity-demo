//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "buzzerResource.h"

void BuzzerResource::put(OCRepresentation& recv_rep)
{
	debugPrint({"in PUT"});
	recv_rep.getValue("buzzer", tone);
	debugPrint({"tone: ", to_string(tone)});
}

