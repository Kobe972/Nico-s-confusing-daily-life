#pragma once
#include<Windows.h>
class CPlayer
{
public:
	void MoveNext();
	char* GetSkinFile(int SkinID);
	int m_PlayerState;
	int m_PlayerDetail;//more accurate state
	int m_PlayerSkin;//skin id
	char* m_skin;//skin file(bmp)
	int m_Position_x,m_Position_y;
	int m_Height,m_Width;
	RECT m_boarder;
};

