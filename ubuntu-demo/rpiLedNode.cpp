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
	return false;
}

bool RpiLedNode::putEnable()
{
	return true;
}

void RpiLedNode::putDataToRep(OCRepresentation& rep)
{
	rep.setValue("red", red);
	rep.setValue("green", green);
	rep.setValue("blue", blue);
}
