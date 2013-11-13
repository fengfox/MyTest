#include "StdAfx.h"
#include "DataControl.h"
#include "myList.h"
#include "tinyxml.h"
#pragma comment(lib,"tinyxml.lib")
DataControl::DataControl(void)
{
}


DataControl::~DataControl(void)
{
}
bool DataControl::save(CString path)
{
	
}
 myList DataControl::load(CString path)
 {
	 myList mylist;
	 //´Óxml¶Á¡£
	 TiXmlDocument doc;     
	 doc.LoadFile(path);
	 TiXmlElement* firstLevel=doc.RootElement();
	 if(firstLevel!=NULL)
	 {

	 }
	

 }