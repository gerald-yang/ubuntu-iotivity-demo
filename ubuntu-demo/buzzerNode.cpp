//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "buzzerNode.h"

bool BuzzerNode::getEnable()
{
	return false;
}

bool BuzzerNode::putEnable()
{
	return true;
}

void BuzzerNode::putDataToRep(OCRepresentation& rep)
{
	rep.setValue("buzzer", buzzer);
}
