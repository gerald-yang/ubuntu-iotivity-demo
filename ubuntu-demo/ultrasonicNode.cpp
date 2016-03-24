//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "ultrasonicNode.h"

void UltrasonicNode::getDataFromRep(const OCRepresentation& rep)
{
	rep.getValue("ultrasonic", ultrasonic);
	
	cout << debugInfo << "ultrasonic: " << to_string(ultrasonic) << endl;
}

