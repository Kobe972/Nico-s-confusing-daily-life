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
//button states
#define BSTATENORMAL 0
#define BSTATEON 1
#define BSTATEDOWN 2
#define BSTATEUP 3


//checkbox ID
#define JSILENCE 1
//checkbox environment variable
#define JSILENCE_X 400 
#define JSILENCE_Y 300
#define JSILENCE_WIDTH 116
#define JSILENCE_HEIGHT 39
//checkbox states
#define CSTATEON 0
#define CSTATEOFF 1
class CButton : CGameEntry
{
public:
	~CButton();
	void init_by_ID(int ID);
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

class CINPUTBOX : CGameEntry
{
public:
	CINPUTBOX();			//构造函数可以代替init的功能
	RECT boarder[3]; //依次为鼠标未移过、鼠标移过、鼠标按下时的按钮边界
	char* m_Style[2];//各状态时的图标地址（激活、未激活）
	char m_input[50];
	int m_ID;

public:
	void Draw();
};
