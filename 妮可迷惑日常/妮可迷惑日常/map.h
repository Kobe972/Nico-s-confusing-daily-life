#pragma once
#include "GameEntry.h"
#define SILENCEGAPHIGH 500
#define SILENCEGAPLOW 300
#define SILENCEPLANEHIGH 300
#define SILENCEPLANELOW 100

#define SOFIELD 0 
#define SOPILLAR 1
#define SOMATHBOOK 2
#define MAPSPEED 18
const int ModelCounter[3] = { 10, 10, 10 };
class CStaticObstacle
{
public:
	RECT m_safe;  //if collision detection in BOB is bad
	void Create(bool safetop, bool safebottom, bool safeleft, bool saferight, std::string filename);

private:
	//figture information
	LPDIRECTDRAWSURFACE7 m_Surface;

public:
	void Draw(RECT border);
};
class CMap
{
public:
	CMap() {};
	void Create(int AllModelLength, int Hardness, int SilenceLow, int Silencehigh);
	void GetRandMap();
	void RecvMap(char* Msg);//receive map data from the server;

public:
	void Draw();
	bool MoveNext();
public:
	RECT m_CurBarrier[5000];
	RECT m_CurSafe[5000];
	int m_BarrierNumber;
	void GetCurBarrier();

private:
	//int Load_Model_Hardnessx_Idy(int left);
	void AddField(int left, int right, int height);
	void AddStObstacle(int left, int right, int low, int height, int kind);
	int BaseHardV(int left);
	int BaseHardUUDD(int left);
	int BaseHardW(int left);
	int BaseHardJump(int left);
	int Load_Model_Hardness2_Id0(int left);
	int Load_Model_Hardness2_Id1(int left);
	int Load_Model_Hardness2_Id2(int left);
	int Load_Model_Hardness2_Id3(int left);
	int (CMap::* m_pfnLoad_Model[3][10])(int) = {
		{},
		{},
		{&CMap::Load_Model_Hardness2_Id0, &CMap::Load_Model_Hardness2_Id1, &CMap::Load_Model_Hardness2_Id2,
		&CMap::Load_Model_Hardness2_Id3}
	};
	void Load_Silence(int left, int Length);

private:
	int m_Length;
	int m_Left; //left border
	int m_Right; //right border

	int m_ObstacleCounter; //number of obstacles
	int m_ObstacleCur;
	int m_ObstacleKind[5000];
	RECT m_Obstacle[5000];

	int m_fieldCounter;
	int m_fieldCur;
	RECT m_field[5000];
};

