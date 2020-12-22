#pragma once
#include"map.h"
#include"GUI.h"
#include"player.h"
#include <Windows.h>
//then define game states
#define PREFACE (EGameState::eGameStatePreface)
#define MAINMENU (EGameState::eGameStateMainMenu)
#define SELECT_SKIN (EGameState::eGameStateSelectSkin)
#define SELECT_HARDNESS (EGameState::eGameStateSelectHardness)
#define WAITOTHERS (EGameState::eGameStateWaitOthers)
#define SETTINGS (EGameState::eGameStateSettings)
#define HELP (EGameState::eGameStateHelp)
#define LOG (EGameState::eGameStateLogin)
#define REGISTRY (EGameState::eGameStateRegister) //the ID of the button is IREGISTRY,be aware of it
#define SEE_RANKS (EGameState::eGameStateShowRank)
#define PRELUDE (EGameState::eGameStatePrelude)
#define SINGLE_PLAYER_BEGIN (EGameState::eGameStateSinglePlayer)
#define SINGLEFAILURE (EGameState::eGameStateSingleFailure)
#define SINGLESUCCESS (EGameState::eGameStateSingleSuccess)
//prelude ID
#define ISIGN_NAME 0
#define IMATH_TEST 1
#define ICONFESSION 2
#define I2048 3
#define ICARD 4


#pragma warning
EXTERN_BOB_OBJECTS()
EXTERN_INPUT_DATA()

extern CButton button[20];
extern CCheckBox checkbox[20];
extern CInputBox inputbox[5];
extern CStaticObstacle staticobstacle[20];
class CGame
{
public:
	void GameInit();
	void GameMain();


	~CGame();


private:
	CPlayer m_player;
	CPlayer m_opponents[10];
	int m_OpponentCnt;
	int m_OpponentProceses; //from 1 to 100,indicates how much
private: //game state
	bool m_IsSilent = false;

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
	enum EGameState {
		eGameStatePreface = 0, eGameStateMainMenu = 1, eGameStateSelectSkin, eGameStateSelectHardness, eGameStateSettings, eGameStateHelp,
		eGameStateLogin, eGameStateRegister, eGameStateShowRank, eGameStatePrelude, eGameStateSinglePlayer, eGameStateSingleFailure, eGameStateSingleSuccess,
		eGameStateWaitOthers, eGameStateMultiPlayer,
		eGameStateWaitToEnd,
		eGameStateCount
	};
	//GameState 望文生义

private:
	EGameState m_eGameState{ CGame::EGameState::eGameStateMainMenu };

public:

	void Preface();
	void ConnectToServ();//connect to server
	void GetCurMsg();
	void ProcessButtonMsg();//处理按钮事件
	void ProcessCheckBoxMsg();//处理单选框事件
	void ProcessKeyMsg();
	void ShowMenu();
	void EnterFrame();//绘制游戏下一帧
	void SinglePlayer();//开始
	void SingleFailure();
	void SingleSuccess();
	void MultiPlayer();
	void Help();
	void Settings();
	void Login();//登录
	void Reg();//注册账号
	void Prelude(); //单人游戏开始前的任务介绍
	void Appendix();//单人游戏通关后的任务执行（子游戏）
	void WaitOthers();//双人游戏开始前等待其他玩家加入
	void ShowRank();//双人游戏结束后显示排名
	void WaitToEnd();//双人游戏等待结束以及结束后的数据处理工作
	void SelectSkin();
	void SelectHardship();
	void RecvSerMessage();//接收服务器发来的信号
	void ProcessSerMessage(char* Msg);//处理信号
	void SendMsg(char* Msg);//给服务器发送信号

	//A set of functions that can set arguments.
public:
	inline void SetGameState(EGameState eGameStateCurrent);
	inline void SetWindowHandle(HWND hwnd);
public:
	int m_prelude_ID = -1; //not enter the state
	int m_prelude_frame;
	const int frame_number[5] = { 10,10,6,2,3 };//frame number for every prelude ID
private:
	HWND m_hWnd;

private:
	//不同阶段初始化函数和函数指针数组

private:
	//不同阶段绘图函数和函数指针数组

private:
	//不同阶段逻辑运行函数和函数指针数组

};

