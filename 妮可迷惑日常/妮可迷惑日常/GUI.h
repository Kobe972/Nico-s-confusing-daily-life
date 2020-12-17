#pragma once
#include "GameEntry.h"
#include"BobClass.h"
//button ID
#define ISINGLE_MODE 0
#define IMULTIMODE 1
#define ISETTINGS 2
#define IHELP 3
#define ILOG 4
#define IREGISTRY 5
#define IRETURN 6
#define ISEE_RANKS 7
#define IPETERSKIN 8
#define ISATASKIN 9
#define ITINGTINGSKIN 10
#define IMBGSKIN 11
#define ICWKSKIN 12
#define IOK_SELECTHARDSHIP 13
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

//inputbox ID
#define IHARDSHIPBOX 0
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
	bool m_clipped=false;//之前鼠标是否按下
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
