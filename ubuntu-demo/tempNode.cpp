//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "tempNode.h"

void TempNode::getDataFromRep(const OCRepresentation& rep)
{
	rep.getValue("temperature", temperature);
	cout << debugInfo << "temperature: " << to_string(temperature) << endl;
}
