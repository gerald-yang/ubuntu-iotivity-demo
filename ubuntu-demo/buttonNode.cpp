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
	cout << debugInfo << to_string(button) << endl;
}
