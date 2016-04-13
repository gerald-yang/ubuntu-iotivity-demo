//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef RPILEDNODE_H
#define RPILEDNODE_H

#include "baseNode.h"

class RpiLedNode : public BaseNode
{
public:
	RpiLedNode(string _requestName, string _requestUri) 
		: BaseNode(_requestName, _requestUri) 
	{
		red = 0;
		green = 0;
		blue = 0;
	};

	bool getEnable();
	bool putEnable();
	void putDataToRep(OCRepresentation& rep);

	int red;
	int green;
	int blue;
};

#endif
