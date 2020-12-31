#pragma once
#include "GameEntry.h"
#define SILENCEGAPHIGH 500
#define SILENCEGAPLOW 300
#define SILENCEPLANEHIGH 300
#define SILENCEPLANELOW 100

#define SOFIELD 0 
#define SOPILLAR Rand(1, 3)
#define SORANDUPDANGER Rand(4, 7)
#define SORANDUPSAFE Rand(8, 12)
#define SOJUAN 13
#define SOYINGJUAN 14

#define MOCONFUSE 0
#define MONICO 1
#define Rand(L, R) (rand() % (R - L + 1) + L)
class CStaticObstacle
{
public:
	RECT m_safe;  //if collision detection in BOB is bad
	int m_HPReduce;
	void Create(int safetop, int safebottom, int safeleft, int saferight, int reduce, std::string filename);

private:
	//figture information
	LPDIRECTDRAWSURFACE7 m_Surface;

public:
	void Draw(RECT border);
};

class CMovingObstacle
{
public:
	RECT m_safe;  //if collision detection in BOB is bad
	int m_Speed;
	int m_HPReduce;
	void Create(int safetop, int safebottom, int safeleft, int saferight, 
		int reduce, int speed, std::string filename);
private:
	//figture information
	LPDIRECTDRAWSURFACE7 m_Surface;

public:
	void Draw(RECT border);
};

class CHP
{
public:
	void Load_Frame(std::string filename);

private:
	//figture information
	LPDIRECTDRAWSURFACE7 m_Surface[11];

public:
	void Draw(RECT border, int hp);
};


class CMap
{
public:
	CMap() {};
	void Create(int AllModelLength, int Hardness, int SilenceLow, int Silencehigh);
	void CreateMulti(int ModelNumber, char* Hardness, char* ModelID);
	void GetRandMap();
	void RecvMap(char* Msg);//receive map data from the server;

public:
	void Draw(int hp);
	int Fix();
	bool MoveNext();
public:
	RECT m_CurBarrier[5000];
	RECT m_CurSafe[5000];
	int m_BarrierNumber;

private:
	//int Load_Model_Hardnessx_Idy(int left);
	void AddField(int left, int right, int height);
	void AddStObstacle(int left, int right, int low, int height, int kind); 
	void AddMoObstacle(int left, int right, int low, int height, int kind);
	int Model_For_Fools(int left);
	int Load_Easy_Model(int left);
	int Load_Model_Hardness0_Id0(int left);
	int Load_Model_Hardness0_Id1(int left);
	int Load_Model_Hardness0_Id2(int left);
	int Load_Model_Hardness0_Id3(int left);
	int Load_Model_Hardness0_Id4(int left);
	int Load_Model_Hardness0_Id5(int left);
	int Load_Model_Hardness0_Id6(int left);
	int Load_Model_Hardness0_Id7(int left);
	int Load_Model_Hardness0_Id8(int left);
	int Load_Model_Hardness0_Mixed(int left);
	int Load_Model_Hardness0_Mixed_Advanced(int left);


	int Load_Model_Hardness1_Id0(int left);
	int Load_Model_Hardness1_Id1(int left);
	int Load_Model_Hardness1_Id2(int left);
	int Load_Model_Hardness1_Id3(int left);
	int Load_Model_Hardness1_Id4(int left);
	int Load_Model_Hardness1_Id5(int left);

	int BaseHardV(int left);
	int BaseHardUUDD(int left);
	int BaseHardW(int left);
	int BaseHardJump(int left);
	int BaseHardCLIMBE(int left);
	int BaseHardCHOICE(int left);
	int BaseHardBIU(int left);
	int BaseHardNICONICONI(int left);
	int BaseHardNICONINICO(int left);
	int BaseHardNICOCONINI(int left);
	int Load_Model_Hardness2_Id0(int left);
	int Load_Model_Hardness2_Id1(int left);
	int Load_Model_Hardness2_Id2(int left);
	int Load_Model_Hardness2_Id3(int left);
	int Load_Model_Hardness2_Id4(int left);
	int Load_Model_Hardness2_Id5(int left);
	int Load_Model_Hardness2_Id6(int left);
	int Load_Model_Hardness2_Id7(int left);
	int Load_Model_Hardness2_NICO(int left);
	int (CMap::* m_pfnLoad_Model[3][10])(int) = {
		{&CMap::Load_Model_Hardness0_Id1,
		&CMap::Load_Model_Hardness0_Id2,
		&CMap::Load_Model_Hardness0_Id3,
		&CMap::Load_Model_Hardness0_Id4,
		&CMap::Load_Model_Hardness0_Id5,
		&CMap::Load_Model_Hardness0_Id6,
		&CMap::Load_Model_Hardness0_Id7,
		&CMap::Load_Model_Hardness0_Id8,
		&CMap::Load_Model_Hardness0_Mixed,
		&CMap::Load_Model_Hardness0_Mixed_Advanced},

		{&CMap::Load_Model_Hardness1_Id0,
		&CMap::Load_Model_Hardness1_Id1,
		&CMap::Load_Model_Hardness1_Id2,
		&CMap::Load_Model_Hardness1_Id3,
		&CMap::Load_Model_Hardness1_Id4,
		&CMap::Load_Model_Hardness1_Id5},

		{&CMap::Load_Model_Hardness2_Id0,
		&CMap::Load_Model_Hardness2_Id1,
		&CMap::Load_Model_Hardness2_Id2,
		&CMap::Load_Model_Hardness2_Id3,
		&CMap::Load_Model_Hardness2_Id4,
		&CMap::Load_Model_Hardness2_Id5,
		&CMap::Load_Model_Hardness2_Id6,
		&CMap::Load_Model_Hardness2_Id7,
		&CMap::Load_Model_Hardness2_NICO}
	};
	void Load_Silence(int left, int Length);

public:
	int m_Length;
	int m_Left; //left border
	int m_Right; //right border

private:

	int m_ObstacleCounter; //number of obstacles
	int m_ObstacleCur;
	int m_ObstacleKind[5000];
	RECT m_Obstacle[5000];

	int m_fieldCounter;
	int m_fieldCur;
	RECT m_field[5000];

	int m_MovingCounter; //number of obstacles
	int m_MovingCur;
	int m_MovingKind[5000];
	RECT m_Moving[5000];
};

