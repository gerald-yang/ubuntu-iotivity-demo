//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef GROVETEMPNODE_H
#define GROVETEMPNODE_H

#include "baseNode.h"

class GroveTempNode : public BaseNode
{
public:
	GroveTempNode(string _requestName, string _requestUri) 
		: BaseNode(_requestName, _requestUri) 
	{
		temperature = 0.0;
	};

	bool getEnable();
	bool putEnable();
	void getDataFromRep(const OCRepresentation& rep);
	double temperature;
};

#endif
