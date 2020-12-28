#pragma once
#include "GameEntry.h"
#include"BobClass.h"
//button ID
#define ISINGLE_MODE 0
#define IMULTIMODE 1
#define ISETTINGS 2
#define IHELP 3
#define ILOG 4
#define IACHIEVEMENT 5
#define IREGISTRY 6
#define IRETURN 7
#define ISEE_RANKS 8
#define IPETERSKIN 9
#define ISATASKIN 10
#define ITINGTINGSKIN 11
#define IMBGSKIN 12
#define ICWKSKIN 13
#define IOK_SELECTHARDSHIP 14
#define IOK_MATH_TEST 15

//button states
#define BSTATENORMAL 0
#define BSTATEON 1
#define BSTATEDOWN 2
#define BSTATEUP 3


//checkbox ID
#define JSILENCE 1
//checkbox environment variable
#define JSILENCE_X 70 
#define JSILENCE_Y 200
#define JSILENCE_WIDTH 116
#define JSILENCE_HEIGHT 39
//checkbox states
#define CSTATEON 0
#define CSTATEOFF 1

//achievement id
#define ACJUANWANG 0 // have meet 1000 juan
#define ACSTOP 1 //have meet 500 ying juan
#define ACWARMMING 2 //have play 100000 meters
#define ACMASTER 3 //have play 1000000 meters
#define ACNICO 4 //have crash 100 nico
#define ACCONFUSE 5 // have crash 100 confuse
#define ACNICOCONFUSE 6 // have crash 10 nico and 10 confuse in one run
#define ACMATHKING 7 // have pass 100 math and 10 confuse in one run
#define ACMATHFAIL 8 // have crash 100 math and 10 confuse in one run
#define ACFORCEKING 9 // have pass 100 math and 10 confuse in one run
#define ACFORCEFAIL 10 // have crash 100 math and 10 confuse in one run
#define ACBIOLOGYKING 11 // have pass 100 math and 10 confuse in one run
#define ACBIOLOGYFAIL 12 // have crash 100 math and 10 confuse in one run


//inputbox ID
#define IHARDSHIPBOX 0
#define IMATH_ANSBOX 1//used in math test
class CButton : CGameEntry
{
public:
	~CButton();
	void Create(int ID, int width, int height, int x, int y, std::string filename);
	RECT boarder; //按钮边界
	int m_Width, m_Height;
	char* m_Style[3];//各状态时的图标地址（平时、鼠标移过、按下）
	LPDIRECTDRAWSURFACE7 m_ButtonSur[3];
	BITMAP_FILE m_bitmap[3];
	int m_ID;
	int m_state;
	bool m_clipped = false;//之前鼠标是否按下
public:
	void Draw();
	void Check();//process its reaction toward mouse
};

class CCheckBox : CGameEntry
{
public:
	~CCheckBox();
	void Create(int ID, int width, int height, int x, int y, std::string filename, bool state);
	RECT boarder; //按钮边界
	int m_Width, m_Height;
	char* m_Style[2];
	LPDIRECTDRAWSURFACE7 m_CheckBoxSur[2];
	BITMAP_FILE m_bitmap[2];
	int m_ID;
	int m_state;
	int m_ClipTime = GetTickCount(); //last mouse clip time
public:
	void Draw();
	void Check();//process its reaction toward mouse
};

class CInputBox : CGameEntry
{
public:
	void Create(int ID, int width, int height, int x, int y, bool password);
	bool m_password;
	RECT boarder; //边界
	int m_Width, m_Height;
	bool m_activated = false;
	char m_input[50];
	char m_last_input = 0;
	int m_ID;
	clock_t m_last_input_time;

public:
	void Draw();
	void Check();
};

class CAchievement : CGameEntry
{
public:
	std::string m_title;
	std::string m_context;
	int m_x, m_y;
	LPDIRECTDRAWSURFACE7 m_Surface;
	void Create(int x, int y, const char* title, const char* context);
public:
	void Draw();
};
