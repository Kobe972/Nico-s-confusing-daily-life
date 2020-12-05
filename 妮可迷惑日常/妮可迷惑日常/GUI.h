#pragma once
#include "GameEntry.h"

class CButton : CGameEntry
{
public:
	CButton();			//构造函数可以代替init的功能
	RECT boarder[3]; //依次为鼠标未移过、鼠标移过、鼠标按下时的按钮边界
	char* m_Style[3];//各状态时的图标地址（平时、鼠标移过、按下）
	int m_ID;

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