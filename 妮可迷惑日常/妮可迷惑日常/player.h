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
	int vx=5, vy=5;//�ٶ�
	int x, y;//��������ʾ����λ�ã�ԭ��Ϊ���½ǣ���������Ƹ�����ѧ���㣬����ǰ׺
private:
	int m_PlayerState;
	int m_PlayerDetail;				//more accurate state
	int m_PlayerSkin;				//skin id
	char* m_skin;					//skin file(bmp)
	//Size and location was defined in the class CGameEntry
	RECT m_boarder;					//ͼ��߽磨���Σ�

private:
	//����ѻ�ͼ��صĶ�����������
};
