#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <iostream>
#include <stack>
#include <math.h>
#include <vector>
#include <sstream>

using namespace std;

enum XMLProperties { objectopen,
							propertyopen,
							attributeopen,
							actionopen,
							layoutopen, 
							asline,
							opened, closed
						 };

multimap<string, string> fillObjectsMap(multimap<string, string> map);
multimap<string, string> fillComponentsMap(multimap<string, string> map);
multimap<string, string> fillXMLopenMap(multimap<string, string> map);
multimap<string, string> fillXMLcloseMap(multimap<string, string> map);
multimap<string, string> fillEventsMap(multimap<string, string> map);
string getObjectName(string objname);
string getObjectClass(string classes);
string getPropertyName(string prop);
string getPropertyValue(string value);
string cutSpaces(string str);
string replaceInvalidChars(string str);
double round(double zahl, unsigned int stellen);
double computePixelHeight(double ppi, double fh);
string convertToLower(string str);
void writeLines(XMLProperties prop, string propname, string objectname);
multimap<string, string> writePropertyGeometry(multimap<string, string> map);
multimap<string, string> writePropertyFont(multimap<string, string> map);
// TODO: next line have been never work before
//multimap<string, string> writePropertyBoolean(multimap<string, string> map, string map);
void writeObjectProperty(multimap<string, string> map);
void writeMenuItem(vector<string> menu);
void writeConnections(multimap<string, string> sensig, multimap<string,string> recslo);
void writeCloseWidget(int wids);

int objclosed = 0;					// für Test (Anzahl geschlossener Objekte / gleich mit zaehler)
stack<string> objsave;
stack<string> objnamesave;
vector<string> menu;
vector<string> actions;
multimap <string,string> objectsmmap;
multimap <string,string> propertiesmmap;
multimap <string,string> saveobjectsmmap;						 
multimap <string,string> xmlopenmmap;
multimap <string,string> xmlclosemmap;
multimap <string,string> eventsmmap;


ifstream ifile("RMonitor.dfm");  // Source datei
ofstream ofile("RMonitor.xml");   // Destination datei

//**********************************************************************************************

multimap<string, string> fillObjectsMap(multimap<string, string> map)
{
	map.insert(pair<string,string>("TForm1", 			"QWidget"));
	map.insert(pair<string,string>("TCVdrForm", 		"QWidget"));
	map.insert(pair<string,string>("TRmForm", 		"QWidget"));					//???
	map.insert(pair<string,string>("TButton", 		"QPushButton"));
	map.insert(pair<string,string>("TBitBtn", 		"QPushButton"));
	map.insert(pair<string,string>("TLabel", 			"QLabel"));
	map.insert(pair<string,string>("TEdit", 			"QLineEdit"));
	map.insert(pair<string,string>("TGroupBox", 		"QGroupBox"));
	map.insert(pair<string,string>("TCheckBox", 		"QCheckBox"));
	map.insert(pair<string,string>("TRadioButton", 	"QRadioButton"));
	//map.insert(pair<string,string>("TRadioGroup", 	"QGroupBox"));
	map.insert(pair<string,string>("TBevel", 			"Line"));
	map.insert(pair<string,string>("TPageControl", 	"QTabWidget"));
	map.insert(pair<string,string>("TTabSheet", 		"QWidget"));
        map.insert(pair<string,string>("TImage", 			"QGraphicsView"));
	map.insert(pair<string,string>("TStatusBar", 	"QStatusBar"));
	map.insert(pair<string,string>("TScrollBox", 	"QScrollArea"));
	map.insert(pair<string,string>("TProgressBar", 	"QProgressBar"));	
	//map.insert(pair<string,string>("TOpenDialog", 	"QFileDialog"));
	map.insert(pair<string,string>("TMainMenu", 		"QMenuBar"));					// QMenuBar ???
	map.insert(pair<string,string>("TMenuItem", 		"QMenu"));
	//map.insert(pair<string,string>("TMenuItem", 		"QMenu"));				// addaction name=actionQuit/seperator
	map.insert(pair<string,string>("TComboBox", 		"QComboBox"));
	map.insert(pair<string,string>("TTimer", 			"QTimer"));      	
	map.insert(pair<string,string>("TCheckListBox", "QListWidget"));   
	//map.insert(pair<string,string>("TListBox", 		"QListView"));				//????
	//map.insert(pair<string,string>("TMemo", 			"QTextEdit"));				//???? vorhanden
	//map.insert(pair<string,string>("TStaticText", 	"QLabel"));				//????
	//map.insert(pair<string,string>("TCGauge", 		"QGauge"));				//????
	//map.insert(pair<string,string>("TCompas", 		"QCompas"));       		//????
	//map.insert(pair<string,string>("TMer", 			"QMer"));					//????						
	//map.insert(pair<string,string>("TMJWcrt", 		"TMJWcrt"));				//????
	map.insert(pair<string,string>("TPanel", 			"QFrame"));       		//????
	map.insert(pair<string,string>("TCSpinButton",	"QSpinBox"));
		
	return map;	
}

//**********************************************************************************************

multimap<string, string> fillPropertiesMap(multimap<string, string> map)
{
	map.insert(pair<string,string>("Left", 			"x"));
	map.insert(pair<string,string>("Top", 				"y"));
	map.insert(pair<string,string>("Height", 			"height"));
	map.insert(pair<string,string>("Width", 			"width"));
	map.insert(pair<string,string>("ClientHeight", 	"height"));
	map.insert(pair<string,string>("ClientWidth", 	"width"));
	map.insert(pair<string,string>("Font.Height", 	"pointsize"));
	map.insert(pair<string,string>("Font.Name", 		"family"));
	map.insert(pair<string,string>("Caption", 		"string"));
	map.insert(pair<string,string>("Caption", 		"title"));
	map.insert(pair<string,string>("Caption", 		"windowTitle"));
//	map.insert(pair<string,string>("Visible", 		"visible"));			//damit im Designer sichtbar
//	map.insert(pair<string,string>("Enabled", 		"enabled"));
	map.insert(pair<string,string>("Hint", 			"toolTip"));
//	map.insert(pair<string,string>("PixelsPerInch", "pixelsperinch"));
//	map.insert(pair<string,string>("Sorted", 			"sortingEnabled"));
	map.insert(pair<string,string>("Items.Strings", "item"));				// Listen einer ComboBox	
//	map.insert(pair<string,string>("OnClick", 		"OnClick"));			// eventsmap
	
	return map;	
}

//**********************************************************************************************

multimap<string, string> fillXMLopenMap(multimap<string, string> map)
{
	map.insert(pair<string,string>("ui", 				"<ui version=\"4.0\" >"));						
	map.insert(pair<string,string>("classes", 		"<class >"));					
	map.insert(pair<string,string>("widget", 			"<widget class=\"*\" name=\"*\" >"));
	map.insert(pair<string,string>("property", 		"<property name=\"*\" >"));
	map.insert(pair<string,string>("attribute", 		"<attribute name=\"*\" >"));
	map.insert(pair<string,string>("action", 			"<action name=\"action*\" >"));
	map.insert(pair<string,string>("iconset", 		"<iconset resource=\"*\" >"));
	map.insert(pair<string,string>("include", 		"<include location=\"*\" >"));
	map.insert(pair<string,string>("layout", 			"<layout class=\"*\" >"));
	map.insert(pair<string,string>("rect", 			"<rect>"));
	map.insert(pair<string,string>("x", 				"<x>"));
	map.insert(pair<string,string>("y", 				"<y>"));
	map.insert(pair<string,string>("width", 			"<width>"));       		
	map.insert(pair<string,string>("height", 			"<height>"));
	map.insert(pair<string,string>("font", 			"<font>"));
	map.insert(pair<string,string>("pointsize",		"<pointsize>"));
	map.insert(pair<string,string>("weight", 			"<weight>"));
	map.insert(pair<string,string>("bold", 			"<bold>"));
	map.insert(pair<string,string>("string", 			"<string>"));	
	map.insert(pair<string,string>("bool", 			"<bool>"));							
	map.insert(pair<string,string>("enum", 			"<enum>"));				
	map.insert(pair<string,string>("item", 			"<item>"));						
	map.insert(pair<string,string>("resources", 		"<resources>"));
	map.insert(pair<string,string>("connections", 	"<connections>"));
	map.insert(pair<string,string>("connection", 	"<connection>"));					
	map.insert(pair<string,string>("sender", 			"<sender>"));				
	map.insert(pair<string,string>("signal", 			"<signal>"));
	map.insert(pair<string,string>("receiver", 		"<receiver>"));						
	map.insert(pair<string,string>("slot", 			"<slot>"));  
		
	return map;	
}

//**********************************************************************************************

multimap<string, string> fillXMLcloseMap(multimap<string, string> map)
{
	map.insert(pair<string,string>("ui", 				"</ui>"));						
	map.insert(pair<string,string>("classes", 		"</class>"));					
	map.insert(pair<string,string>("widget", 			"</widget>"));
	map.insert(pair<string,string>("property", 		"</property>"));
	map.insert(pair<string,string>("attribute", 		"</attribute>"));
	map.insert(pair<string,string>("action", 			"</action>"));
	map.insert(pair<string,string>("iconset", 		"</iconset>"));
	map.insert(pair<string,string>("include", 		"</include>"));
	map.insert(pair<string,string>("layout", 			"</layout>"));
	map.insert(pair<string,string>("rect", 			"</rect>"));
	map.insert(pair<string,string>("x", 				"</x>"));
	map.insert(pair<string,string>("y", 				"</y>"));
	map.insert(pair<string,string>("width", 			"</width>"));       				
	map.insert(pair<string,string>("height", 			"</height>"));
	map.insert(pair<string,string>("font", 			"</font>"));
	map.insert(pair<string,string>("poinzsize",		"</pointsize>"));
	map.insert(pair<string,string>("weight", 			"</weight>"));
	map.insert(pair<string,string>("bold", 			"</bold>"));
	map.insert(pair<string,string>("string", 			"</string>"));	
	map.insert(pair<string,string>("bool", 			"</bool>"));					
	map.insert(pair<string,string>("enum", 			"</enum>"));				
	map.insert(pair<string,string>("item", 			"</item>"));						
	map.insert(pair<string,string>("resources", 		"</resources>"));
	map.insert(pair<string,string>("connections", 	"</connections>"));
	map.insert(pair<string,string>("connection", 	"</connection>"));					
	map.insert(pair<string,string>("sender", 			"</sender>"));				
	map.insert(pair<string,string>("signal", 			"</signal>"));
	map.insert(pair<string,string>("receiver", 		"</receiver>"));						
	map.insert(pair<string,string>("slot", 			"</slot>")); 
	
	return map;	
}

//**********************************************************************************************

multimap<string, string> fillEventsMap(multimap<string, string> map)
{
	map.insert(pair<string,string>("OnClick", 		"Clicked()"));
	//map.insert(pair<string,string>("OnEnter", 		"</iconset>"));
	//map.insert(pair<string,string>("OnKeyDown", 		"</include>"));
	map.insert(pair<string,string>("OnKeyPress",		"Pressed()"));
	map.insert(pair<string,string>("OnKeyUp", 		"Released()"));
	//map.insert(pair<string,string>("OnMouseDown", 	"</x>"));
	//map.insert(pair<string,string>("OnMouseMove", 	"</y>"));
	map.insert(pair<string,string>("OnMouseUp", 		"Released()"));	
	
	return map;
}

//**********************************************************************************************

string getObjectName(string objname)
{
	objname = objname.substr((objname.find_first_of("t", 0)+2), objname.length());
	objname = objname.substr(0, objname.find_first_of(":", 0));
	return objname;
}

//***********************************************************************************************

string getObjectClass(string classes)
{
	classes = classes.substr((classes.find_first_of(":", 0)+2), classes.length());
	return classes;
}

//************************************************************************************************

string getPropertyName(string prop)
{
	prop = prop.substr(0, prop.find_first_of("=", 0));
	return prop;
}

//*************************************************************************************************

string getPropertyValue(string value)
{
	value = value.substr((value.find_first_of("=", 0)+1), value.length());
	return value;
}

//*************************************************************************************************

string cutSpaces(string str)
{
	string::size_type loc, loc1;
	
	loc = str.find(" ", 0);
	while(loc != string::npos)
	{
		loc1 = str.find("=",0);
		if(loc < (loc1+2))
		{
			str.replace(loc, 1, "");
			loc = str.find(" ", loc);
		}
		else
		{
			break;
		}
	}
	
	loc = str.find("'", 0);
	while(loc != string::npos)
	{
		str.replace(loc, 1, "");
		loc = str.find("'", loc);
	}
	
	return str;
}

//*************************************************************************************************

string cutAllNameSpacesAndInvChars(string str)
{
	string::size_type loc;
	
	while((loc = str.find(" ", 0)) != string::npos)
	{
		str.replace(loc, 1, "");
	}	
	while((loc = str.find(".")) != string::npos)
	{
		str.replace(loc, 1, "");
	}
	while((loc = str.find(",")) != string::npos)
	{
		str.replace(loc, 1, "");
	}
	while((loc = str.find("+")) != string::npos)
	{
		str.replace(loc, 1, "pos");
	}
	while((loc = str.find("-")) != string::npos)
	{
		str.replace(loc, 1, "neg");
	}
	return str;	
}

//*************************************************************************************************

string replaceInvalidChars(string str)
{
	string::size_type loc;
	while((loc = str.find("<")) != string::npos)
	{
		str.replace(loc, 1, "[");
	}
	while((loc = str.find(">")) != string::npos)
	{
		str.replace(loc, 1, "]");
	}
	
	return str;
}

//*************************************************************************************************

double round(double zahl, unsigned int stellen)
{
    zahl *= pow(10, stellen);
    if (zahl >= 0)
        zahl = floor(zahl + 0.5);
    else
        zahl = ceil(zahl - 0.5);
    zahl /= pow(10, stellen);
    return zahl;
} 

//**************************************************************************************************

double computePixelHeight(double ppi, double fh)
{
	fh = fh*72/ppi;
	fh = fh*(-1);
	return round(fh, 0);
}

string convertToLower(string str)
{
	char c;
	int i = 0;
	while (str[i])
		{
		   c = str[i];
		   str[i] = tolower(c);
		   i++;
		}
	return str;
}

//**************************************************************************************************

void writeLines(XMLProperties prop, string propname, string objectname)
{
	int pos;
	string str;
	multimap<string,string>::iterator iter;
	switch(prop)
	{
		case objectopen:		iter = xmlopenmmap.find("widget");
									if(iter != xmlopenmmap.end())
									{
										str = iter->second;
										pos = str.find("*", 0);
										str = str.replace(pos, 1, propname, 0, str.length());
										pos = str.find("*", (pos + 1));
										str = str.replace(pos, 1, objectname, 0, str.length());
										ofile << str << endl;
										//cout << str << endl;	
									}	
									break;
		case propertyopen: 	iter = xmlopenmmap.find("property");
									if(iter != xmlopenmmap.end())
									{
										str = iter->second;
										str.replace(str.find("*", 0), 1, propname, 0, str.length());	
										ofile << str << endl;
										//cout << str << endl;		
									}
									break;
		case attributeopen: 	iter = xmlopenmmap.find("attribute");
									if(iter != xmlopenmmap.end())
									{
										str = iter->second;
										str.replace(str.find("*", 0), 1, propname, 0, str.length());	
										ofile << str << endl;
										//cout << str << endl;		
									}
									break;
		case actionopen: 	iter = xmlopenmmap.find("action");
									if(iter != xmlopenmmap.end())
									{
										str = iter->second;
										propname = cutAllNameSpacesAndInvChars(propname);		// Leerzeichen aus Bezeichner entfernen
										//propname = replaceInvalidChars(propname);
										str.replace(str.find("*", 0), 1, propname, 0, str.length());	
										ofile << str << endl;
										//cout << str << endl;		
									}
									break;
		case layoutopen: break;
		case asline:			iter = xmlopenmmap.find(propname);
									str = iter->second;
									iter = xmlclosemmap.find(propname);
									ofile << str << objectname << iter->second << endl;
									//cout << str << objectname << iter->second << endl;;
									break;		
		case opened : 			iter = xmlopenmmap.find(propname);
									if(iter != xmlopenmmap.end())
									{
										str = iter->second;	
										ofile << str << endl;		
										//cout << str << endl;
									}
									break;
		case closed: 			iter = xmlclosemmap.find(propname);
									if(iter != xmlclosemmap.end())
									{
										
										str = iter->second;	
										ofile << str << endl;
										//cout << str << endl;		
									}
									break;	
	}
}

//*********************************************************************************************

multimap<string, string> writePropertyGeometry(multimap<string, string> map)
{
	multimap<string,string>::iterator iter;
	int value = 0;
	bool boollayout = false;
	string str;
	stringstream sstr;
	if(objsave.size() == 2 && objsave.top() != "QMenuBar")
		boollayout = true;
	writeLines(propertyopen, "geometry", "");
	writeLines(opened, "rect", "");
	
	iter = map.find("x");
	if(iter != map.end()) 
	{
     	ofile << "<" << iter->first << ">" << iter->second << "</" << iter->first << ">" << endl;
     	map.erase(iter);
   }
   iter = map.find("y");
   if(iter != map.end()) 
	{
		str = iter->second;
		sstr << str;
		sstr >> value;
		if(boollayout)
			value = value + 25;
     	ofile << "<" << iter->first << ">" << value << "</" << iter->first << ">" << endl;
     	map.erase(iter);
   }
   iter = map.find("width");
   if(iter != map.end()) 
	{
     	ofile << "<" << iter->first << ">" << iter->second << "</" << iter->first << ">" << endl;
     	map.erase(iter);
   }
   iter = map.find("height");
   if(iter != map.end()) 
	{
     	ofile << "<" << iter->first << ">" << iter->second << "</" << iter->first << ">" << endl;
     	map.erase(iter);
   }
	
	writeLines(closed, "rect", "");
	writeLines(closed, "property","");
	
	if((iter = map.find("x")) != map.end())
		map.erase(iter);
	if((iter = map.find("y")) != map.end())
		map.erase(iter); 
	if((iter = map.find("width")) != map.end())
		map.erase(iter);
	if((iter = map.find("height")) != map.end())
		map.erase(iter);
	
	return map;
}

//*********************************************************************************************

multimap<string, string> writePropertyFont(multimap<string, string> map)
{
	multimap<string,string>::iterator iter;
	
	writeLines(propertyopen, "font","");
	writeLines(opened, "font","");
	
	iter = map.find("bold");
	if(iter != map.end()) 
	{
     	ofile << "<" << iter->first << ">" << iter->second << "</" << iter->first << ">" << endl;
     	map.erase(iter);
   }
   iter = map.find("weight");
	if(iter != map.end()) 
	{
     	ofile << "<" << iter->first << ">" << iter->second << "</" << iter->first << ">" << endl;
     	map.erase(iter);
   }
   iter = map.find("pointsize");
	if(iter != map.end()) 
	{
     	ofile << "<" << iter->first << ">" << iter->second << "</" << iter->first << ">" << endl;
     	map.erase(iter);
   }
   iter = map.find("family");
	if(iter != map.end()) 
	{
     	ofile << "<" << iter->first << ">" << iter->second << "</" << iter->first << ">" << endl;
     	map.erase(iter);
   }
   
   writeLines(closed, "font","");
   writeLines(closed, "property","");
   
   return map;
}

//*********************************************************************************************

multimap<string, string> writePropertyBoolean(multimap<string, string> map, string prop)
{
	multimap<string,string>::iterator iter;
	string property = "bool";
	string value;
	if(prop == "item")
		writeLines(propertyopen, "text", "");
	else
		writeLines(propertyopen, prop, "");
	if(prop == "toolTip" || prop == "item")
		property = "string";
	iter = map.find(prop);
	if(iter != map.end()) 
	{
		value = convertToLower(iter->second);
		//writeLines(asline, property, value);
		ofile << "<" << property << ">" << value << "</" << property << ">" << endl;
     	map.erase(iter);
   }
   writeLines(closed, "property","");
   
   return map;
}

//*********************************************************************************************

void writeObjectProperty(multimap<string, string> map)
{
	string act;
	string strclose;
	string strletter;
	string::size_type loc;
	multimap<string,string>::iterator iter;
   if(!map.empty())
   {
	   if((iter = map.find("x")) != map.end() &&
		(iter = map.find("y")) != map.end() &&
		(iter = map.find("width")) != map.end() &&
		(iter = map.find("height")) != map.end())
		{
			map = writePropertyGeometry(map);
		}
		else
		{
			if((iter = map.find("x")) != map.end())
				map.erase(iter);
			if((iter = map.find("y")) != map.end())
				map.erase(iter);
			if((iter = map.find("width")) != map.end())
				map.erase(iter);
			if((iter = map.find("height")) != map.end())
				map.erase(iter);
		}
	   
	   if((iter = map.find("bold")) != map.end() ||
		(iter = map.find("weight")) != map.end() ||
		(iter = map.find("pointsize")) != map.end() ||
		(iter = map.find("family")) != map.end())
		{
			map = writePropertyFont(map);
		} 

		if((iter = map.find("sortingEnabled")) != map.end())
			map = writePropertyBoolean(map, "sortingEnabled");
		
		if((iter = map.find("visible")) != map.end())
			map = writePropertyBoolean(map, "visible");

		if((iter = map.find("enabled")) != map.end())
			map = writePropertyBoolean(map, "enabled");

		if((iter = map.find("toolTip")) != map.end())
			map = writePropertyBoolean(map, "toolTip");
		
	   if((iter = map.find("addaction")) != map.end())
		{
			map.erase(iter);
			objclosed++;
			if((iter = map.find("string")) != map.end())
			{	//<addaction name="actionNew" />
				if(iter->second == "-")
					ofile << "<addaction name=\"seperator\"/>" << endl;
				else
				{
					act = cutAllNameSpacesAndInvChars(iter->second);
					ofile << "<addaction name=\"action" << act << "\"/>" << endl;
					actions.push_back(iter->second);
				}
				map.erase(iter);
			}
		}  
	   
	   if((iter = map.find("string")) != map.end())
		{
			if(objsave.top() == "Form")
			{
				strclose = "property";
				writeLines(propertyopen, "windowTitle","");
			}
			else
			{
				if(objsave.top() == "QGroupBox" || objsave.top() == "QMenu")
				{
					strclose = "property";
					writeLines(propertyopen, "title","");
				}
				else
				{
					if(objsave.top() == "QWidget")
					{
						strclose = "attribute";
						writeLines(attributeopen, "title", "");
					}
					else
					{
						if(objsave.top() == "QFrame")
						{
							strclose = "property";
							writeLines(propertyopen, "toolTip", "");
						}
						else
						{
							strclose = "property";
							writeLines(propertyopen, "text","");
						}
					}
				}	
			}
			strletter = iter->second;
			loc = strletter.find("&", 0); // umwandeln "&&" in "&amp;"
			if(loc != string::npos)
			{
				strletter.replace(loc+1, 1, "amp;");
			}
			
			ofile << "<" << iter->first << ">" << strletter << "</" << iter->first << ">" << endl;
			map.erase(iter);	
			writeLines(closed, strclose, "");	
		}
		
		while((iter = map.find("item")) != map.end())
			{
				writeLines(opened, "item", "");
				map = writePropertyBoolean(map, "item");
				writeLines(closed, "item", "");
			}
				
	   for( iter = map.begin(); iter != map.end(); iter++ ) 
		{			
			if(iter->first == "widget")
			{
				writeLines(closed, "widget", "");
			}
			else
			{
		   	ofile << "<" << iter->first << ">" << iter->second << "</" << iter->first << ">" << endl;
			}
		}
   }
}

//**********************************************************************************

void writeMenuItem(vector<string> menu)
{
	vector<string>::iterator iter;
	while(!menu.empty())
	{
		iter = menu.begin();
		ofile << *iter << endl;
		menu.erase(iter);
	}
}

//**********************************************************************************

void writeActions(vector<string> act)
{
	vector<string>::iterator iter;
	while(!act.empty())
	{
		iter = act.begin();
		writeLines(actionopen, *iter, "");
		writeLines(propertyopen, "text", "");
		writeLines(asline, "string", *iter);
		writeLines(closed, "property", "");
	  	writeLines(closed, "action", "");
	  	act.erase(iter);
	}
}

//**********************************************************************************

void writeConnections(multimap<string,string> sensig, multimap<string,string> recslo)
{
	string str;
	multimap<string,string>::iterator ssiter;
	multimap<string,string>::iterator rsiter;
	writeLines(opened, "connections", "");
	while(!sensig.empty() && !recslo.empty())
	{
		ssiter = sensig.begin();
		rsiter = recslo.begin();
		writeLines(opened, "connection", "");
		writeLines(asline, "sender", ssiter->first);
		writeLines(asline, "signal", ssiter->second);
		writeLines(asline, "receiver", "");
		str = rsiter->first;
		str.append("()");		// Funtionsdefinition
		writeLines(asline, "slot", str);
    	writeLines(closed, "connection", "");
    	sensig.erase(ssiter);
    	recslo.erase(rsiter);
	}
	writeLines(closed, "connections", "");
}

//**********************************************************************************

void writeCloseWidget(int wids)
{
	int i = 0;
	while(wids > 0)
	{
		if(i == 1)
			writeMenuItem(menu);
		if(i == 2)
			writeActions(actions);			// Namen der Actions beinhalten Zahlen am Anfang
		objclosed++;
		writeLines(closed, "widget", "");
		//ofile << "########################################################################################" << endl;
		wids--;
		i++;
	}
}

//*****************************  MAIN  **********************************************

int oldMain()
{
bool parseobject = true; 		// Prüfung ob Objekt geparst werden kann
bool ismenu = false;
bool ismenuitem = false;
bool readmod = false;
char strin[10];					// zum konvertieren#
int i = 0;
int menucount = 0;
int counter = 0;					// für nicht parsebar objekte
int pan = 0;						// für Panels als Property Liste
int clwidget = 0;					// für noch zu schliessende Objekte
int objopened = 0;					// für Test (Anzahl der geöffneten Objekte / gleich mit zaehler1)
double pixels = 0.0;				// zur Berechnung der Textgröße (PixelperInch)
double textheight = 0.0;		// zur Berechnung der Textgröße (Font.Height)
string str;
string substr;
string menuitem;
stringstream sstr;
string::size_type loc;
string::size_type loco;
//vector<string> items;
multimap <string,string> sendersignalmmap;
multimap <string,string> receiverslotmmap;
multimap<string,string>::iterator ssiter;

	objectsmmap = fillObjectsMap(objectsmmap);
	propertiesmmap = fillPropertiesMap(propertiesmmap);
	xmlopenmmap = fillXMLopenMap(xmlopenmmap);
	xmlclosemmap = fillXMLcloseMap(xmlclosemmap);
	eventsmmap = fillEventsMap(eventsmmap);

	if(!ifile)
	{
		//cout << "Konnte Datei nicht öffnen" << endl;
		ofile << "Konnte Datei nicht öffnen" << endl;
	}
	
	ofile << "<ui version=\"4.0\" >" << endl;
	ofile << "<class>RmForm</class>" << endl;	 //bestimmbar durch eingabe der Datei
	
	while(getline(ifile, str))
	{
		if(readmod)
		{
			str = cutSpaces(str);
			cout << str << endl;
			loc = str.find(")" , (str.size()-1));
			if(loc != string::npos)
			{
				readmod = false;
				str = str.replace(loc, 1, "");
				saveobjectsmmap.insert(pair<string, string>("item", str));
			}
			else
				saveobjectsmmap.insert(pair<string, string>("item", str));
			continue;
		}
		
		loc = str.find("\r", 0);
		if(loc != string::npos)
		{
			str = str.substr(0,(str.length())-1);// -1 = Zeilenumbruch entfernen
		}
		loc = str.find("object", 0);
		loco = str.find(":", 0);
		if(loc != string::npos && loco != string::npos)
		{	
			writeObjectProperty(saveobjectsmmap);		// Propertyspeicher schreiben
			saveobjectsmmap.clear();						// Propertyspeicher leeren	
			
			//writeCloseWidget(clwidget);				// weitere Objekte abschließen
			while(!objsave.empty() && clwidget > 0 && parseobject)
			{
				objclosed++;
				writeLines(closed, "widget", "");
				//cout << "END: " << objsave.top();
				objsave.pop();
				//cout << ": " << objsave.size() << endl;
				clwidget--;	
				if(objsave.top() == "QMenuBar")
					ismenu = true;
			}
			if(ismenu)
			{
				writeMenuItem(menu);
				menu.clear();
				ismenu = false;
			}
			clwidget = 0;			
			
			multimap<string,string>::iterator iter = objectsmmap.find(getObjectClass(str));
			if(iter != objectsmmap.end())
			{
				objopened++;
				parseobject = true;
				
				if(i == 0)
					objsave.push("Form");
				else
					objsave.push(iter->second);			//Object in den Stack schreiben
				objnamesave.push(getObjectName(str));	//Object name in den Stack schreiben
				
				//Menueinträge 
				if(iter->second == "QMenuBar")
				{
					ismenu = true;
					writeLines(objectopen, iter->second, getObjectName(str));
				}
				else
				{
					if(iter->second == "QMenu")
					{
						ismenuitem = true;
						if(menucount == 0)
						{
							writeLines(objectopen, iter->second, getObjectName(str));
							menuitem = "<addaction name=\"" + getObjectName(str) + "\" />";
							menu.push_back(menuitem);
						}
						else
							objsave.pop();							
						menucount++;
					}
					else
					{
						//cout << iter->second << ": " << objsave.size() << endl;
						writeLines(objectopen, iter->second, getObjectName(str));
					}
				}
			}
			else
			{
				// wenn Object nicht parsebar dann keine Properties auslesen
				parseobject = false;
				counter++;
			}	
		}
		else
		{
			int len = str.length();
			str = cutSpaces(str); 				//Leerzeichen entfernen
			if((loc = str.find("Panels", 0)) != string::npos && parseobject)
			{
				pan = 1;	
			}	
				
			loc = str.find("end", 0);
			if(loc != string::npos && pan == 0 && str.size() == 3)		//&& parseobject 
			{
				if(parseobject)
				{
					//Menuitems beenden
					if(menucount > 0)
					{
						menucount--;
						if(menucount == 0)
						{
							clwidget++;
						}
					}
					else
						clwidget++;
				}
				else
				{
					if(counter == 1)
					{
						parseobject = true;
					}
					counter--;
				}
				//break;			//Verkürzen der Ausführung bis zum ersten "end"
			}
			else
			{
				loc = str.find("object", 0);
				if(loc == string::npos && pan == 0 && parseobject)
				{		
					str = replaceInvalidChars(str);
					
					multimap<string,string>::iterator iter = propertiesmmap.find(getPropertyName(str));
					if(iter != propertiesmmap.end())
					{
						if(iter->second == "pixelsperinch")
						{
							pixels = atof(getPropertyValue(str).c_str());							
						}
	
						if(pixels != 0 && textheight != 0)
						{
							textheight = computePixelHeight(pixels, textheight);
							sprintf(strin, "%d", (int)textheight);
							saveobjectsmmap.insert(pair<string, string>("pointsize", strin));
							textheight = 0;
						}
						// Objektnamen und Event speichern für connections
						if((ssiter = eventsmmap.find(iter->second)) != eventsmmap.end())
						{
							sendersignalmmap.insert(pair<string,string>(objnamesave.top(), ssiter->second ));
							receiverslotmmap.insert(pair<string,string>(getPropertyValue(str), ""));
							//cout << "sen: " << objnamesave.top() << " | sig: " << ssiter->second << endl;						
						}
						
						if(iter->second == "item")
						{
							substr = getPropertyValue(str);
							substr = cutSpaces(substr);
							loc = substr.find("(", 0);
							if(loc != string::npos)
							{
								readmod = true;
							}
							continue;							
						}
						
						if(iter->second == "pointsize")
						{
							sstr << getPropertyValue(str);
							sstr >> textheight;
						}
						else
						{
							if(ismenu)
							{
								saveobjectsmmap.insert(pair<string, string>("x", "0"));
								saveobjectsmmap.insert(pair<string, string>("y", "0"));
								saveobjectsmmap.insert(pair<string, string>("width", "500"));
								saveobjectsmmap.insert(pair<string, string>("height", "25"));
								ismenu = false;
							}
							else
							{
								if(ismenuitem)
								{
									if(menucount > 1)
									{
										saveobjectsmmap.insert(pair<string, string>("addaction", getPropertyValue(str)));
									}
									saveobjectsmmap.insert(pair<string, string>(iter->second, getPropertyValue(str)));
								}
								else
									saveobjectsmmap.insert(pair<string, string>(iter->second, getPropertyValue(str)));
							}
						}					
					}
				}
			}
			
			if((loc = str.find_last_of(">", len)) != string::npos && parseobject)
			{
				pan = 0;
			}	
		}
		i++;
	}
	
	writeObjectProperty(saveobjectsmmap);		// Propertyspeicher schreiben
	saveobjectsmmap.clear();						// Propertyspeicher leeren	
	writeCloseWidget(objsave.size());			// die letzten Objekte abschließen
	writeConnections(sendersignalmmap, receiverslotmmap);			// Signale und Slots einfügen (Events) 
	ofile << "</ui>" << endl;
	ifile.close();
	ofile.close();
	cout << "objekte geöffnet: " << objopened << endl;
	cout << "objeket geschlossen: " << objclosed << endl;
	return 0;
}

