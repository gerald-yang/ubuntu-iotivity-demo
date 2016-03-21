//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "groveTempNode.h"

bool GroveTempNode::getEnable()
{
	return true;
}

bool GroveTempNode::putEnable()
{
	return false;
}

void GroveTempNode::getDataFromRep(const OCRepresentation& rep)
{
	rep.getValue("temperature", temperature);
}

