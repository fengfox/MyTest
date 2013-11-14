#pragma once
#include "myItem.h"

class myList
{
public:
	myList(void);
	~myList(void);
	myItem *item;
	void deleteList();
};

