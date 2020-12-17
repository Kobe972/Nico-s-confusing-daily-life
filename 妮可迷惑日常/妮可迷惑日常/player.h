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
	//�˴���ö��
	enum EPlayerState {
		ePlayerStateHoldingOn = 0, ePlayerStateRunning, ePlayerStateJumping,
		ePlayerStateAirHiking, ePlayerStateSliding,
		ePlayerStateDefault, ePlayerStateCount
	};

public:
	/*
			ʹ�÷�ʽ��
			����UP����Jump����������Down����Slide������
			ÿ֡ʹ��MoveNext(),Draw(),LogicRun();

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
	char* GetSkinFile(int SkinID);								//������
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
	//����ѻ�ͼ��صĶ�����������
	int m_PictureCounter = 0; //the number of picture we load in
	int* m_FrameList[PlayerSkinCounter][4];
	// 0 for the holdingon 1 for run 2 for jump 3 for slide
	int m_FrameNumber[PlayerSkinCounter][4]; //the number of pictrue
	int m_Interval = 0;

	//BOB����ʵ����

	int Collision(RECT* barrier, RECT* save, int BarrierNumber);
	void GetCurBorder(RECT* broder);
public: ///��ɾ��
	//load in one animation
	void Load_Once_Animation(std::string filename, int animate_index, int filenumber,
		int* Tlist, BITMAP_FILE_PTR Tcurmap, int next_frame = -1);

private:
	//�˶��߼�����
	//��Ϊ����BOB�� ��Ҫ�����˶���ʼʱ��BOB���������á�
	//�����ں����ڲ����þ�̬������ ��ֹ�˶�δ����ʱ�ظ���ʼ��
	void MoveHoldingOn(void);					//�����׶��˶���������������������Ҫ����λ��
	void MoveRunning(void);						//�ܶ��׶��˶���������������
	void MoveJumping(void);						//��Ծ�׶��˶���������������
	void MoveAirHiking(void);						//�������׶��˶���������������
	void MoveSliding(void);						//�����׶��˶���������������
	void MoveFalling(void);
	/*void DrawDefault(void) {}					//�޶���

	//			����ָ�����鷽����ã�ע���ʽ�� һ�������ܴ�����
	void (CPlayer::*m_pfnDrawFunction[EPlayerState::ePlayerStateCount])(void) = {
		&CPlayer::DrawHoldingOn,
		&CPlayer::DrawRunning,
		&CPlayer::DrawJumping,
		&CPlayer::DrawAirHiking,
		&CPlayer::DrawSliding,
		&CPlayer::DrawDefault };*/

private:
	//int m_iRemainingActionFrame{ 0 };			//��ǰ����ʣ�༸֡����
};