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

private:
	int m_PlayerState;
	int m_PlayerDetail;				//more accurate state
	int m_PlayerSkin;				//skin id
	char* m_skin;					//skin file(bmp)
	//Size and location was defined in the class CGameEntry
	RECT m_boarder;					//����Ӹ�ע�ͺ���

private:
	//����ѻ�ͼ��صĶ�����������
};
