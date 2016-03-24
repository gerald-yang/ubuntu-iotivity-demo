//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef LEDNODE_H
#define LEDNODE_H

#include "baseNode.h"

class LedNode : public BaseNode
{
public:
	LedNode(string _requestName, string _requestUri) 
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
