#pragma once
#include "pch.h"
class CPoint
{
	int m_x, m_y;
};
class CGameEntry
{
private:
	DWORD m_dwX, m_dwY;				//左上角坐标

	DWORD m_dwHeight, m_dwWidth;	//显式地展现大小

public:
	virtual void Draw(void) = 0;//Draw function, must be completed.
	inline void SetLocation(DWORD x, DWORD y);
	inline void SetSize(DWORD height, DWORD width);

};


class CGameEntryMoveable : public CGameEntry
{
public:
	virtual void MoveNext() = 0;	//可动实体需要移动
	inline void SetSpeed(int vx, int vy);

private:
	int m_vx, m_vy;					//可动实体有运行速度
	
};

