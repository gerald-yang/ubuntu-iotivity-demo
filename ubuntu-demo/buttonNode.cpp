//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "buttonNode.h"

bool ButtonNode::getEnable()
{
	return false;
}

bool ButtonNode::putEnable()
{
	return true;
}

void ButtonNode::putDataToRep(OCRepresentation& rep)
{
	rep.setValue("button", button);
}
