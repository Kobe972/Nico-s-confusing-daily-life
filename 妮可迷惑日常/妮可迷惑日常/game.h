#pragma once
#include"map.h"
#include"GUI.h"
#include"player.h"
#include <Windows.h>
#include"SaveTools.h"
#include"client.h"
#include<math.h>
//then define game states
#define PREFACE (EGameState::eGameStatePreface)
#define MAINMENU (EGameState::eGameStateMainMenu)
#define SELECT_SKIN (EGameState::eGameStateSelectSkin)
#define SELECT_HARDNESS (EGameState::eGameStateSelectHardness)
#define WAITOTHERS (EGameState::eGameStateWaitOthers)
#define SETTINGS (EGameState::eGameStateSettings)
#define HELP (EGameState::eGameStateHelp)
#define LOGIN (EGameState::eGameStateLogin)
#define REGISTRY (EGameState::eGameStateRegister) //the ID of the button is IREGISTRY,be aware of it
#define SEE_RANKS (EGameState::eGameStateShowRank)
#define PRELUDE (EGameState::eGameStatePrelude)
#define SINGLEPLAY (EGameState::eGameStateSinglePlayer)
#define SINGLEFAILURE (EGameState::eGameStateSingleFailure)
#define SINGLESUCCESS (EGameState::eGameStateSingleSuccess)
#define SINGLE_PLAYER_BEGIN (EGameState::eGameStateSinglePlayer)
#define ACHIEVEMENT (EGameState::eGameStateAchievement)
#define STATISTICSCOUNT (EGameState::eGameStateStatisticsCount)
#define CONNECTTOSERVE (EGameState::eGameStateConnectToServ)
#define WAITTOCONNECT (EGameState::eGameStateWaitToConnect)
#define MULTIPLAYER (EGameState::eGameStateMultiPlayer)
#define MULTIEND (EGameState::eGameStateMultiEnd)
#define WAITTOEND (EGameState::eGameStateWaitToEnd)
#define SHOW_RANK (EGameState::eGameStateShowRank)

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
extern CAchievement achievement[50];
extern CStaticObstacle staticobstacle[20];
extern CHP hp;


struct RankList {
	int gpa;
	char name[50];
};

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

	CMap m_map;
	int m_CurrentObstacles; //indicates the obstacle the player is going through
	CButton m_button[100];
	SOCKET conn;//socket used to connect to server
private:
	int m_RemainingOpponentCnt;//the number of remained opponents
	int m_hardness;
	bool m_IsSingle;//if it is single player mode
	bool m_state;

public:
	enum EGameState {
		eGameStatePreface = 0, eGameStateMainMenu = 1, eGameStateSelectSkin, eGameStateSelectHardness, eGameStateSettings, eGameStateHelp,
		eGameStateLogin, eGameStateRegister, eGameStateShowRank, eGameStatePrelude, eGameStateSinglePlayer, eGameStateSingleFailure, eGameStateSingleSuccess,
		eGameStateWaitOthers, eGameStateMultiPlayer, eGameStateAchievement, eGameStateConnectToServ,
		eGameStateWaitToEnd, eGameStateStatisticsCount, eGameStateMultiEnd, eGameStateWaitToConnect,
		eGameStateCount
	};
	//GameState ��������

private:
	EGameState m_eGameState{ CGame::EGameState::eGameStateMainMenu };

public:

	void Preface();
	void GetCurMsg();
	bool ButtonReturn();
	void ProcessButtonMsg();//����ť�¼�
	void ProcessCheckBoxMsg();//����ѡ���¼�
	void ProcessKeyMsg();
	void ShowMenu();
	void SinglePlayer();//��ʼ
	void SingleFailure();
	void SingleSuccess();
	void MultiPlayer();
	void Help();
	void Achievement();
	void Statisticscount();
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
	void ConnectToServ();
	void WaitToConnect();
	void MultiEnd();

public: // serve and client
	void ProcessSerMessage();//�����ź�
	void SendMsg(char* Msg);//�������������ź�
	void Load_Rank(int gpa, char* username);
private:
	int m_GPA;
	bool m_loggedin;//whether logged in or not(1 indicates logged in,0 not)
	int m_connected;
	std::vector<RankList> m_ranklist;
	std::string m_LastDie, m_username;

	//A set of functions that can set arguments.
public:
	inline void SetGameState(EGameState eGameStateCurrent);
	inline void SetWindowHandle(HWND hwnd);
public:
	int m_prelude_ID = -1; //not enter the state
	int m_prelude_frame;
	const int frame_number[5] = { 10,10,6,2,3 };//frame number for every prelude ID
	int m_postlude_frame = 0;
	const int fail_frame_number[5] = { 2,3,6,1,1 };
private:
	HWND m_hWnd;
	void play_2048();
	void play_math();
	void play_card();
	void confess();
	void sign_name();
	int fail_2048(); //�����Ƿ��������������0
	int fail_math();
	int fail_card();
	int success_2048();
	int success_math();
	int success_card();

private:
	//��ͬ�׶γ�ʼ�������ͺ���ָ������

private:
	//��ͬ�׶λ�ͼ�����ͺ���ָ������

private:
	//��ͬ�׶��߼����к����ͺ���ָ������

};