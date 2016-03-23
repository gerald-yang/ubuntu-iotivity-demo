//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "humidityNode.h"

void HumidityNode::getDataFromRep(const OCRepresentation& rep)
{
	rep.getValue("humidity", humidity);
}

