//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef BUTTONNODE_H
#define BUTTONNODE_H

#include "baseNode.h"

class ButtonNode : public BaseNode
{
public:
	ButtonNode(string _requestName, string _requestUri) 
		: BaseNode(_requestName, _requestUri) 
	{
		button = 0;
	};

	bool getEnable();
	bool putEnable();
	void getDataFromRep(const OCRepresentation& rep);

	int button;
};

#endif
