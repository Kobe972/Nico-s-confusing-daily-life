#include "pch.h"
#include "player.h"

bool Overlap(RECT a, RECT b) { //watch out that bottom is bigger than top
    return LineCross(a.left, a.right, b.left, b.right) && LineCross(a.top, a.bottom, b.top, b.bottom);
}

bool CPlayer::Create(int frame_number)
{
    // this functino is used to init the m_player bob
    if (!m_Player.Create(PlayerStartX, PlayerStartY, PlayerWidth, PlayerHeight, frame_number,
        BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_ANIM, DDSCAPS_SYSTEMMEMORY))
        return 0;
    m_Player.Set_Anim_Speed(PlayerAnimateSpeed);
    m_ePlayerState = ePlayerStateHoldingOn;
    return 1;
}

void CPlayer::Load_Once_Animation(std::string filename, int animate_index, int filenumber, int* Tlist,
    BITMAP_FILE_PTR Tcurmap, int next_frame)
{
    for (int i = 0; i < filenumber; ++i)
    {
        Tlist[i] = m_PictureCounter;
        Tcurmap->Load_File((filename + std::to_string(i) + ".bmp").c_str());
        m_Player.Load_Frame(Tcurmap, m_PictureCounter++, 0, 0, BITMAP_EXTRACT_MODE_ABS);
        Tcurmap->Unload_File();
    }
    m_Player.Load_Animation(animate_index, filenumber, Tlist, next_frame);
}

void CPlayer::LoadSkinFrame(int SkinID, int RunFrameNumber,
    int AirHikingFrameNumber)
{
    //this funtion is to Load in a new skin to the m_player bob
    BITMAP_FILE Tcurmap;
    int Tlist[MAX_BOB_FRAMES];
    std::string THead = ".\\Picture\\Skin" + std::to_string(SkinID) + "\\"
        + "Skin" + std::to_string(SkinID);

    //load in the holdingon frame
    Load_Once_Animation(THead + "HoldingOn", PlayerAnimateHoldingOn, 1, Tlist, &Tcurmap);

    //load in the Running frame
    Load_Once_Animation(THead + "Running", PlayerAnimateRunning, RunFrameNumber, Tlist, &Tcurmap);

    //load in the Sliding frame
    Load_Once_Animation(THead + "Sliding", PlayerAnimateSliding, 1, Tlist, &Tcurmap);

    //load in the Jumping Launch frame
    Load_Once_Animation(THead + "Jumping", PlayerAnimateJumping, 1, Tlist, &Tcurmap);

    //load in the Jumping Rolling frame
    Load_Once_Animation(THead + "AirHiking", PlayerAnimateAirHiking,
        AirHikingFrameNumber, Tlist, &Tcurmap, PlayerAnimateJumping);
}

void CPlayer::MoveNext()
{
    //move to next animation
    if (m_ePlayerState == ePlayerStateCount) return;
    if (m_Interval++ >= PlayerMovingInterval) {
        m_Interval = 0;
        m_Player.Move();
        m_Player.yv += m_Gravity;
    }
    m_Player.Animate();
}

void CPlayer::Draw()
{
    //此函数将人物图像绘制到窗口后备缓冲表面上
    m_Player.Draw(lpddsback);
    m_Player.Animate();
}

void CPlayer::GetCurBorder(RECT* border) {
    int Twidth, Theight;
    if (m_ePlayerState == ePlayerStateSliding) {
        Twidth = PlayerSlidngWidth;
        Theight = PlayerSlidingHeight;
    }
    else {
        Twidth = PlayerNormalWidth;
        Theight = PlayerNormalHeight;
    }
    border->left = m_Player.x;
    border->right = m_Player.x + Twidth;
    border->bottom = m_Player.y + m_Player.height - Theight;
    border->top = border->bottom - Theight;
}

int CPlayer::Collision(RECT* barrier, RECT* safe, int BarrierNumber)
{
    // this funcion is used to find out whether the collision occurs
    // 0 for collision, 1 for airhiking, 2 for safe collision occurs on the top
    RECT Tcurborder; GetCurBorder(&Tcurborder);

    int TCollisionState = 1;
    for (int i = 0; i < BarrierNumber; ++i)
    {
        RECT Tedge = barrier[i];

        Tedge.right = Tedge.left;
        if (Overlap(Tcurborder, barrier[i])) {
            if (safe[i].left == false)
                return 0;
            else {} //wait for complete
        }
        Tedge.right = barrier[i].right;

        Tedge.left = Tedge.right;
        if (Overlap(Tcurborder, barrier[i])) {
            if (safe[i].right == false)
                return 0;
            else {} //wait for complete
        }
        Tedge.left = barrier[i].left;

        Tedge.bottom = Tedge.top;
        if (Overlap(Tcurborder, barrier[i])) {
            if (safe[i].top == false)
                return 0;
            else TCollisionState = 2;
        }
        Tedge.bottom = barrier[i].bottom;

        Tedge.top = Tedge.bottom;
        if (Overlap(Tcurborder, barrier[i])) {
            if (safe[i].bottom == false)
                return 0;
            else {} //wait for complete
        }
    }
    return TCollisionState;
}

bool CPlayer::LogicRun(RECT* barrier, RECT* safe, int BarrierNumber)
{
    //返回false结束游戏， 返回true继续游戏，此函数应当完成跳跃至落地状态的调整，以及滑和跑的转换。
    int TCurPlayerState = Collision(barrier, safe, BarrierNumber);
    //0 for collision, 1 for airhiking, 2 for running
    if (!TCurPlayerState) return false;

    if (TCurPlayerState == 1 && m_ePlayerState != ePlayerStateAirHiking
        && m_ePlayerState != ePlayerStateJumping) {
        m_ePlayerState = ePlayerStateJumping;
        MoveFalling();
    }

    if (TCurPlayerState == 2 && m_ePlayerState != ePlayerStateRunning) {
        m_ePlayerState = ePlayerStateRunning;
        MoveRunning();
    }
    return true;
}

bool CPlayer::Jump() {
    if (m_ePlayerState == ePlayerStateAirHiking) return 0;
    if (m_ePlayerState == ePlayerStateJumping) {
        m_ePlayerState = ePlayerStateAirHiking;
        MoveAirHiking();
    }
    else {
        m_ePlayerState = ePlayerStateJumping;
        MoveJumping();
    }
    return 1;
}

bool CPlayer::Slide()
{
    if (m_ePlayerState == ePlayerStateJumping || m_ePlayerState == ePlayerStateAirHiking) return 0;
    else {
        m_ePlayerState = ePlayerStateSliding;
        MoveSliding();
    }
    return 1;
}

bool CPlayer::Run()
{
    m_ePlayerState = ePlayerStateRunning;
    MoveRunning();
    return 1;
}

char* CPlayer::GetSkinFile(int SkinID) { return nullptr; }

inline int CPlayer::GetPlayerState() { return m_ePlayerState; }

void CPlayer::MoveHoldingOn(void) {
    m_Player.Set_Animation(PlayerAnimateHoldingOn);
    m_Player.Set_Vel(0, 0);
    m_Gravity = 0;
}

void CPlayer::MoveRunning(void) {
    m_Player.Set_Animation(PlayerAnimateRunning);
    m_Player.Set_Vel(0, 0);
    m_Gravity = 0;
}

void CPlayer::MoveSliding(void) {
    m_Player.Set_Animation(PlayerAnimateSliding);
    m_Player.Set_Vel(0, 0);
    m_Gravity = 0;
}

void CPlayer::MoveJumping(void) {
    m_Player.Set_Animation(PlayerAnimateJumping);
    m_Player.Set_Vel(PlayerJumpVx, PlayerJumpVy);
    m_Gravity = PlayerGravity;
}

void CPlayer::MoveAirHiking(void) {
    m_Player.Set_Animation(PlayerAnimateAirHiking);
    m_Player.Set_Vel(PlayerJumpVx, PlayerJumpVy);
    m_Gravity = PlayerGravity;
}

void CPlayer::MoveFalling(void) {
    m_Player.Set_Animation(PlayerAnimateJumping);
    m_Player.Set_Vel(0, 0);
    m_Gravity = PlayerGravity;
}

/*void CPlayer::DrawHoldingOn(void)
{
}

void CPlayer::DrawRunning(void)
{
}

void CPlayer::DrawJumping(void)
{
}

void CPlayer::DrawAirHiking(void)
{
}

void CPlayer::DrawSliding(void)
{
}
*/