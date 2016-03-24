//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "ledNode.h"

bool LedNode::getEnable()
{
	return false;
}

bool LedNode::putEnable()
{
	return true;
}

void LedNode::putDataToRep(OCRepresentation& rep)
{
	rep.setValue("red", red);
	rep.setValue("green", green);
	rep.setValue("blue", blue);
}
