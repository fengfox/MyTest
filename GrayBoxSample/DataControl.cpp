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
bool DataControl::save(CString path,myList ml)
{
	TiXmlDocument *myDocument = new TiXmlDocument();
	TiXmlElement *ListElement = new TiXmlElement("List");
	myDocument->LinkEndChild(ListElement);
	TiXmlElement *ItemElement = new TiXmlElement("Item");
    ListElement->LinkEndChild(ItemElement);
	
	
		TiXmlElement *symbolElement=new TiXmlElement("symbol");
		
		ItemElement->LinkEndChild(symbolElement);
		TiXmlText *symbolContent=new TiXmlText("IBM");
		symbolElement->LinkEndChild(symbolContent);
		
	myDocument->SaveFile(path);
	return false;
}
 myList DataControl::load(CString  path)
 {
	 myList mylist;
	
	 //´Óxml¶Á¡£
	 TiXmlDocument doc;     
	 doc.LoadFile(path);
	 TiXmlElement* firstLevel=doc.RootElement();
	 int count=0;
	 TiXmlElement* secondLevel=firstLevel->FirstChildElement();
	 while(secondLevel!=NULL)
	 {
		 TiXmlElement* thirdLevel=secondLevel->FirstChildElement();
		 mylist.item[count].symbol=thirdLevel->FirstChild()->Value();
		 mylist.item[count].ATR=thirdLevel->NextSiblingElement()->FirstChild()->Value();
		 mylist.item[count].LOO_ATR=thirdLevel->NextSiblingElement()->NextSiblingElement()->FirstChild()->Value();
		 mylist.item[count].Stop_ATR=thirdLevel->NextSiblingElement()->NextSiblingElement()->NextSiblingElement()->FirstChild()->Value();
		 secondLevel=secondLevel->NextSiblingElement();
		 count++;
	 }
	 return mylist;
	

 }