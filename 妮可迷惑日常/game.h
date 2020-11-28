#pragma once
#include<ddraw.h>
#include "pch.h"
#include"player.h"
#include"map.h"
#include"button.h"

class CDirectDrawObjects
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
class CGame
{
public:
	void GameInit();
	void GameMain();
	
	
	~CGame() {};

	enum EGameState { eGameStateDefault = 0, eGameStateCount };	
	//GameState 最终将Default改成第一个状态， 否则会有问题！！！！！！！

private:
	CPlayer m_player;
	CPlayer m_opponents[10];
	int m_OpponentCnt;
	int m_OpponentProceses; //from 1 to 100,indicates how much 
	
	EGameState eGameState{ EGameState::eGameStateDefault };
	CMap m_map;
	int m_CurrentObstacles; //indicates the obstacle the player is going through
	CDirectDrawObjects m_DDrawStuff;//ddrawobj used in painting
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

	//A set of functions that can set arguments.
public:
	inline void SetGameState(EGameState eGameStateCurrent);
	inline void SetWindowHandle(HWND hwnd);

private:
	HWND m_hWnd;

private:
	//不同阶段初始化函数和函数指针数组

private:
	//不同阶段绘图函数和函数指针数组

private:
	//不同阶段逻辑运行函数和函数指针数组

};

