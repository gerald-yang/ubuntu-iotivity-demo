//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include "soundNode.h"

void SoundNode::getDataFromRep(const OCRepresentation& rep)
{
	rep.getValue("sound", sound);
}

