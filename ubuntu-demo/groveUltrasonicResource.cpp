//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "groveUltrasonicResource.h"


void GroveUltrasonicResource::get()
{
	debugPrint({"in GET"});
	debugPrint({"ultrasonic: ", to_string(ultrasonic)});
	rep.setValue("ultrasonic", ultrasonic);
}

