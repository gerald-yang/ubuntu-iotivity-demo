//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "rpiLedNode.h"

bool RpiLedNode::getEnable()
{
	return true;
}

bool RpiLedNode::putEnable()
{
	return true;
}

void RpiLedNode::getDataFromRep(const OCRepresentation& rep)
{
	rep.getValue("red", red);
	rep.getValue("green", green);
	rep.getValue("blue", blue);
	
	debugPrint({"red: ", to_string(red)});
	debugPrint({"green: ", to_string(green)});
	debugPrint({"blue: ", to_string(blue)});
}

void RpiLedNode::putDataToRep(OCRepresentation& rep)
{
	rep.setValue("red", red);
	rep.setValue("green", green);
	rep.setValue("blue", blue);
}
