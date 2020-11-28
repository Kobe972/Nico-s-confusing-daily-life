#pragma once
#include<Windows.h>
#include "GameEntry.h"

class CButton : CGameEntry
{
public:
	CButton();			//构造函数可以代替init的功能
	RECT boarder[3]; //依次为鼠标未移过、鼠标移过、鼠标按下时的按钮边界
	char* Style[3];//各状态时的图标地址
	int m_ID;

public:
	void Draw();
};