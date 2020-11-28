#pragma once
#include "pch.h"
#include "GameEntry.h"

class CPlayer : CGameEntryMoveable
{
public:
	void MoveNext();
	void Draw();
	char* GetSkinFile(int SkinID);
	inline int GetPlayerState();
	inline int GetPlayerDetail();
	int vx=5, vy=5;//速度
	int x, y;//在离屏显示区的位置（原点为左下角）。可能设计复杂数学运算，不加前缀
private:
	int m_PlayerState;
	int m_PlayerDetail;				//more accurate state
	int m_PlayerSkin;				//skin id
	char* m_skin;					//skin file(bmp)
	//Size and location was defined in the class CGameEntry
	RECT m_boarder;					//图像边界（矩形）

private:
	//建议把绘图相关的东西放在这里
};
