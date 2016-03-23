//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "lightNode.h"

void LightNode::getDataFromRep(const OCRepresentation& rep)
{
	rep.getValue("light", light);
}

