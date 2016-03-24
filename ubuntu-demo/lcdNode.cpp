//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "lcdNode.h"

bool LcdNode::getEnable()
{
	return false;
}

bool LcdNode::putEnable()
{
	return true;
}

void LcdNode::putDataToRep(OCRepresentation& rep)
{
	rep.setValue("lcd", lcd);
}
