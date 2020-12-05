#pragma once
#include "GameEntry.h"
class CObstacle : CGameEntryMoveable
{
public:
	void InitByID(int ID);
	//void MoveNext(); 

private:
	int from, to;//it's the position in offscreen!
	int m_untouchable;//if collision detection in BOB is bad
	bool m_IsAnimate{ false };//might not use
	RECT boarder;

public:
	void Draw();
	void MoveNext();	//only use if m_IsAnimate=true
};
class CField : CGameEntry
{
public:
	void InitByID(int ID);

private:
	int from, to;//it's the position in offscreen!
	RECT boarder;

public:
	void Draw();
};
class CTemplate
{
public:
	void InitByID(int ID);

private:
	CObstacle m_objects[10];
	int m_ObsLen; //number of obstacles
	CField m_field[10];
	int m_FieldLen;//number of fields
};
class CMap
{
public:
	CMap() {};
	void GetRandMap();
	void RecvMap(char* Msg);//receive map data from the server;
	void Merge(CTemplate temp);//connect a template behind this one

private:
	
	int m_ObsLen; //number of obstacles
	CField m_field[1000];
	int m_FieldLen;
	CObstacle m_objects[500];
	CTemplate templ[100];
};

