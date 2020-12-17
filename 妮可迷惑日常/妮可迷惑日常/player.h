#pragma once
#include "GameEntry.h"
#include "BobClass.h"
//define of the information about player
#define PlayerStartX 200
#define PlayerStartY 360
#define PlayerWidth 80
#define PlayerHeight 80
#define PlayerSkinCounter 10 //number of the skin

//move argument
#define PlayerAnimateSpeed 5
#define PlayerJumpVx 20
#define PlayerJumpVy -25
#define PlayerGravity 4
#define PlayerMovingInterval 1
/*
argument the first version
#define PlayerAnimateSpeed 5
#define PlayerJumpVx 20
#define PlayerJumpVy -25
#define PlayerGravity 4
#define PlayerMovingInterval 1
*/

//animate argument
#define PlayerAnimateHoldingOn 0
#define PlayerAnimateRunning 1
#define PlayerAnimateSliding 2
#define PlayerAnimateJumping 3
#define PlayerAnimateAirHiking 4
#define PlayerNormalWidth 80
#define PlayerNormalHeight 80
#define PlayerSlidngWidth 80
#define PlayerSlidingHeight 80


#define LineCross(a, b, c, d) (b >= c && d >= a)
bool Overlap(RECT a, RECT b);

class CPlayer : CGameEntryMoveable
{
public:
	//此处放枚举
	enum EPlayerState {
		ePlayerStateHoldingOn = 0, ePlayerStateRunning, ePlayerStateJumping,
		ePlayerStateAirHiking, ePlayerStateSliding,
		ePlayerStateDefault, ePlayerStateCount
	};

public:
	/*
			使用方式：
			键盘UP调用Jump（）；键盘Down调用Slide（）；
			每帧使用MoveNext(),Draw(),LogicRun();

	*/
	bool Create(int frame_number);
	void MoveNext();
	void Draw();
	void LoadSkinFrame(int SkinID, int RunFrameNumber,
		int AirHikingFrameNumber);
	bool LogicRun(RECT* barrier, RECT* safe, int BarrierNumber);
	bool Jump();
	bool Slide();
	bool Run();
	char* GetSkinFile(int SkinID);								//？？？
	inline int GetPlayerState();
public:
	BOB m_Player;
	int m_Gravity = 0;
	int m_PlayerSkin;				//skin id
private:
	EPlayerState m_ePlayerState{ ePlayerStateDefault };
	int m_PlayerDetail;				//more accurate state
	
	char* m_skin;					//skin file(bmp)
	//Size and location was defined in the class CGameEntry

private:
	//建议把绘图相关的东西放在这里
	int m_PictureCounter = 0; //the number of picture we load in
	int* m_FrameList[PlayerSkinCounter][4];
	// 0 for the holdingon 1 for run 2 for jump 3 for slide
	int m_FrameNumber[PlayerSkinCounter][4]; //the number of pictrue
	int m_Interval = 0;

	//BOB对象实例化

	int Collision(RECT* barrier, RECT* save, int BarrierNumber);
	void GetCurBorder(RECT* broder);
public: ///待删除
	//load in one animation
	void Load_Once_Animation(std::string filename, int animate_index, int filenumber,
		int* Tlist, BITMAP_FILE_PTR Tcurmap, int next_frame = -1);

private:
	//运动逻辑计算
	//因为有了BOB， 主要用于运动开始时对BOB参数的设置。
	//建议在函数内部设置静态变量， 防止运动未结束时重复开始。
	void MoveHoldingOn(void);					//待机阶段运动参数设置与启动，但需要设置位置
	void MoveRunning(void);						//跑动阶段运动参数设置与启动
	void MoveJumping(void);						//跳跃阶段运动参数设置与启动
	void MoveAirHiking(void);						//二段跳阶段运动参数设置与启动
	void MoveSliding(void);						//滑铲阶段运动参数设置与启动
	void MoveFalling(void);
	/*void DrawDefault(void) {}					//无动作

	//			函数指针数组方便调用，注意格式， 一个都不能错。。。
	void (CPlayer::*m_pfnDrawFunction[EPlayerState::ePlayerStateCount])(void) = {
		&CPlayer::DrawHoldingOn,
		&CPlayer::DrawRunning,
		&CPlayer::DrawJumping,
		&CPlayer::DrawAirHiking,
		&CPlayer::DrawSliding,
		&CPlayer::DrawDefault };*/

private:
	//int m_iRemainingActionFrame{ 0 };			//当前动作剩余几帧结束
};