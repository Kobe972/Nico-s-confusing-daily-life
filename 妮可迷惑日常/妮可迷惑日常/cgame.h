#pragma once
#include<ddraw.h>
#include<Windows.h>
#include"player.h"
#include"map.h"
#include"button.h"
class CPoint
{
	int m_x, m_y;
};
class DirectDraw_Objects
{
public:
	LPDIRECTDRAW7         lpdd=NULL;   // dd object
	LPDIRECTDRAWSURFACE7  lpddsprimary = NULL;   // dd primary surface
	LPDIRECTDRAWSURFACE7  lpddsback = NULL;   // dd back surface
	LPDIRECTDRAWSURFACE7  lpddsoffscreen = NULL;   // dd off screen surface
	LPDIRECTDRAWPALETTE   lpddpal = NULL;   // a pointer to the created dd palette
	LPDIRECTDRAWCLIPPER   lpddclipper = NULL;   // dd clipper
	PALETTEENTRY          palette[256];          // color palette
	PALETTEENTRY          save_palette[256];     // used to save palettes
	DDSURFACEDESC2        ddsd;                  // a direct draw surface description struct
	DDBLTFX               ddbltfx;               // used to fill
	DDSCAPS2              ddscaps; // a direct draw surface capabilities struct
};
class cgame
{
public:
	void Game_init();
	void Game_Main();
	void SetWindowHandle(HWND hwnd);
	~cgame() {};
	Player m_player;
	Player m_opponents[10];
	int m_num_of_opponents;
	int m_opponent_proceses; //from 1 to 100,indicates how much 
	int m_GameState;
	Map m_map;
	int m_current_obstacles; //indicates the obstacle the player is going through
	DirectDraw_Objects m_drawstuff;//ddrawobj used in painting
	Button m_button[100];
	SOCKET conn;//socket used to connect to server
private:
	int m_remained_opponents;//the number of remained opponents
	int m_hardship;
	int m_loggedin;//whether logged in or not(1 indicates logged in,0 not)
	int m_connected;
	int m_IsSingle;//if it is single player mode
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
};

