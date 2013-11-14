#include "StdAfx.h"
#include "myList.h"


myList::myList(void)
{
	myList::item=new myItem[256];
	
}


myList::~myList(void)
{
	
}
void myList::deleteList()
{
	delete[] myList::item; 

}