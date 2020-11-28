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
	void ProcessButtonMsg();//处理按钮事件
	void ShowMenu();
	void EnterFrame();//绘制游戏下一帧
	void SinglePlayer();//开始
	void MultiPlayer();
	void Prelude(); //单人游戏开始前的任务介绍
	void Appendix();//单人游戏通关后的任务执行（子游戏）
	void WaitOthers();//双人游戏开始前等待其他玩家加入
	void ShowRank();//双人游戏结束后显示排名
	void WaitToEnd();//双人游戏等待结束以及结束后的数据处理工作
	void SelectSkin();
	void SelectHardship();
	void Login();//登录
	void Reg();//注册账号
	void RecvSerMessage();//接收服务器发来的信号
	void ProcessSerMessage(char* Msg);//处理信号
	void SendMsg(char* Msg);//给服务器发送信号
};

