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
	return true;
}

bool ButtonNode::putEnable()
{
	return false;
}

void ButtonNode::getDataFromRep(const OCRepresentation& rep)
{
	rep.getValue("button", button);
	debugPrint({"pressed: ", to_string(button).c_str()});
}
