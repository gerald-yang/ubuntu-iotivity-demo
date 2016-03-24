//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef LCDNODE_H
#define LCDNODE_H

#include "baseNode.h"

class LcdNode : public BaseNode
{
public:
	LcdNode(string _requestName, string _requestUri) 
		: BaseNode(_requestName, _requestUri) 
	{
		lcd.clear();
	};

	bool getEnable();
	bool putEnable();
	void putDataToRep(OCRepresentation& rep);

	string lcd;
};

#endif
