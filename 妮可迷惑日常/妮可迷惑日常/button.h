#pragma once
#include<Windows.h>
class CButton
{
public:
	void Init();

private:
	RECT boarder[3]; //依次为鼠标未移过、鼠标移过、鼠标按下时的按钮边界
	char* Style[3];//各状态时的图标地址
	int m_ID;
};

