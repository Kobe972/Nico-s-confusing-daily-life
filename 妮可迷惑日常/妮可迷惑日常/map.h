#pragma once
#include<Windows.h>
class Obstacle
{
public:
	void InitByID(int ID);
	void next_movement(); //only use if m_IsAnimate=1
	int from, to;//it's the position in offscreen!
	int m_untouchable;//if collision detection in BOB is bad
	int m_IsAnimate=0;//might not use
	RECT boarder;
};
class Field
{
public:
	void InitByID(int ID);
	int from, to;//it's the position in offscreen!
	RECT boarder;
};
class Template
{
public:
	void InitByID(int ID);
	Obstacle m_objects[10];
	int m_ObsLen; //number of obstacles
	Field m_field[10];
	int m_FieldLen;//number of fields
};
class Map
{
public:
	Template templ[100];
	Map() {};
	void GetRandMap();
	void RecvMap(char* Msg);//receive map data from the server;
	Obstacle m_objects[500];
	int m_ObsLen; //number of obstacles
	Field m_field[1000];
	int m_FieldLen;
	void Merge(Template temp);//connect a template behind this one
};

