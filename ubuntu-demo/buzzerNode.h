//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef BUZZERNODE_H
#define BUZZERNODE_H

#include "baseNode.h"

class BuzzerNode : public BaseNode
{
public:
	BuzzerNode(string _requestName, string _requestUri) 
		: BaseNode(_requestName, _requestUri) 
	{
		buzzer = 0.0;
	};

	bool getEnable();
	bool putEnable();
	void putDataToRep(OCRepresentation& rep);

	double buzzer;
};

#endif
