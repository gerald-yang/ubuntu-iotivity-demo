//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#ifndef LCDRESOURCE_H
#define LCDRESOURCE_H

#include "baseResource.h"

class LcdResource : public BaseResource
{
public:
	LcdResource(string _uniqueID, string _resourceName, string _resourceUri, string _resourceTypeName, bool _isSecure, int _row, int _column) 
		: BaseResource(_uniqueID, _resourceName, _resourceUri, _resourceTypeName, _isSecure)
	{
		row = _row;
		column = _column;
		str.clear();
	};

	void get();
	void put(OCRepresentation& recv_rep);
	int row;
	int column;
	string str;
};

#endif
