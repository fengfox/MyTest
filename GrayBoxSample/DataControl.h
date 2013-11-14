#pragma once
#include "myList.h"
class DataControl
{
public:
	DataControl(void);
	~DataControl(void);
	bool save(CString,myList ml);
	myList load(CString);
};

