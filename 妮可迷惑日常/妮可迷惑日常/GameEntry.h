#pragma once
#include "pch.h"
class CPoint
{
	int m_x, m_y;
};
class CGameEntry
{
private:
	DWORD m_dwX, m_dwY;				//���Ͻ�����

	DWORD m_dwHeight, m_dwWidth;	//��ʽ��չ�ִ�С

public:
	virtual void Draw(void) = 0;//Draw function, must be completed.
	inline void SetLocation(DWORD x, DWORD y);
	inline void SetSize(DWORD height, DWORD width);

};


class CGameEntryMoveable : public CGameEntry
{
public:
	virtual void MoveNext() = 0;	//�ɶ�ʵ����Ҫ�ƶ�
	inline void SetSpeed(int vx, int vy);

private:
	int m_vx, m_vy;					//�ɶ�ʵ���������ٶ�
	
};

