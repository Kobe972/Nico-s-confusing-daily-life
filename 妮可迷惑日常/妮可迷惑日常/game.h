#pragma once
#include<ddraw.h>
#include"map.h"
#include"GUI.h"
#include"player.h"
#include <Windows.h>
//then define game states
#define MAINMENU 0
#pragma warning
EXTERN_BOB_OBJECTS()
class CGame
{
public:
	void GameInit();
	void GameMain();
	
	
	~CGame() {};


private:
	CPlayer m_player;
	CPlayer m_opponents[10];
	int m_OpponentCnt;
	int m_OpponentProceses; //from 1 to 100,indicates how much 
	
	int m_state;
	CMap m_map;
	int m_CurrentObstacles; //indicates the obstacle the player is going through
	CButton m_button[100];
	SOCKET conn;//socket used to connect to server
private:
	int m_RemainingOpponentCnt;//the number of remained opponents
	int m_hardness;
	bool m_loggedin;//whether logged in or not(1 indicates logged in,0 not)
	int m_connected;
	bool m_IsSingle;//if it is single player mode

public:

	void ConnectToServ();//connect to server
	void OnMouseMove(UINT nFlags, CPoint point);
	void LeftButtonUp(UINT nFlags, CPoint point);
	void ProcessCurMsg();
	void ProcessKeyMsg();
	void ProcessButtonMsg();//����ť�¼�
	void ShowMenu();
	void EnterFrame();//������Ϸ��һ֡
	void SinglePlayer();//��ʼ
	void MultiPlayer();
	void Prelude(); //������Ϸ��ʼǰ���������
	void Appendix();//������Ϸͨ�غ������ִ�У�����Ϸ��
	void WaitOthers();//˫����Ϸ��ʼǰ�ȴ�������Ҽ���
	void ShowRank();//˫����Ϸ��������ʾ����
	void WaitToEnd();//˫����Ϸ�ȴ������Լ�����������ݴ�����
	void SelectSkin();
	void SelectHardship();
	void Login();//��¼
	void Reg();//ע���˺�
	void RecvSerMessage();//���շ������������ź�
	void ProcessSerMessage(char* Msg);//�����ź�
	void SendMsg(char* Msg);//�������������ź�

	//A set of functions that can set arguments.
public:
	inline void SetGameState(int GameState);
	inline void SetWindowHandle(HWND hwnd);

private:
	HWND m_hWnd;

private:
	//��ͬ�׶γ�ʼ�������ͺ���ָ������

private:
	//��ͬ�׶λ�ͼ�����ͺ���ָ������

private:
	//��ͬ�׶��߼����к����ͺ���ָ������

};

