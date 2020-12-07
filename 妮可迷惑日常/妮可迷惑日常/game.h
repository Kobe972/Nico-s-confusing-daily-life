#pragma once
#include<ddraw.h>
#include"map.h"
#include"GUI.h"
#include"player.h"
#include <Windows.h>
//#include"T3DLIB2.h"
//then define game states
#define MAINMENU 0
#define SELECT_SKIN 1
#define SELECT_HARDSHIP 2
#define SETTINGS 3
#define HELP 4
#define LOG 5
#define REGISTRY 6 //the ID of the button is IREGISTRY,be aware of it
#define SEE_RANKS 7
#pragma warning
EXTERN_BOB_OBJECTS()
extern CButton button[20];
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
	void GetCurMsg();
	void ProcessButtonMsg();//����ť�¼�
	void ProcessKeyMsg();
	void ShowMenu();
	void EnterFrame();//������Ϸ��һ֡
	void SinglePlayer();//��ʼ
	void MultiPlayer();
	void Help();
	void Settings();
	void Login();//��¼
	void Reg();//ע���˺�
	void Prelude(); //������Ϸ��ʼǰ���������
	void Appendix();//������Ϸͨ�غ������ִ�У�����Ϸ��
	void WaitOthers();//˫����Ϸ��ʼǰ�ȴ�������Ҽ���
	void ShowRank();//˫����Ϸ��������ʾ����
	void WaitToEnd();//˫����Ϸ�ȴ������Լ�����������ݴ�����
	void SelectSkin();
	void SelectHardship();
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

