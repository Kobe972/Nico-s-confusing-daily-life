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
#define ISEE_RANKS 6
//button states
#define BSTATENORMAL 0
#define BSTATEON 1
#define BSTATEDOWN 2
#define BSTATEUP 3
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
