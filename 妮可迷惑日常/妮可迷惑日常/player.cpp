#include "pch.h"
#include "player.h"
extern bool g_IsSilent;
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
    m_JumpClick = 0;
    m_PictureCounter = 0;
    m_Interval = 0;
    m_HP = 60;
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
    m_PlayerSkin = SkinID;
    BITMAP_FILE Tcurmap;
    int Tlist[MAX_BOB_FRAMES];
    std::string THead = ".\\Skin\\Skin" + std::to_string(SkinID) + "\\"
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
        if((m_ePlayerState == ePlayerStateJumping || m_ePlayerState == ePlayerStateAirHiking) 
            && m_UnderBorder != 600)
            m_Player.yv = min(m_Player.yv, m_UnderBorder - m_Player.y - PlayerHeight);
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
    border->top = m_Player.y + m_Player.height - Theight;
    border->bottom = border->top + Theight;
}

void CPlayer::GetUnder(RECT* barrier, RECT* safe, int BarrierNumber)
{
    // this funcion is used to find out whether the collision occurs
    // 0 for collision, 1 for airhiking, 2 for safe collision occurs on the top
    RECT Tcurborder; GetCurBorder(&Tcurborder);
    m_UnderBorder = 600;

    for (int i = 0; i < BarrierNumber; ++i)
        if (LineCross(barrier[i].left, barrier[i].right, Tcurborder.left, Tcurborder.right) &&
            barrier[i].top >= Tcurborder.bottom)
            m_UnderBorder = min(m_UnderBorder, barrier[i].top);
}

int Collision_One(RECT barrier, RECT safe, RECT Tcurborder)
{
    //0 for not collision
    //1 for safely collision
    //2 for hp-- collision
    //3 for directly die
    int T_CollisionState = 0;
    RECT Tedge = barrier;

    Tedge.bottom = Tedge.top;
    if (Overlap(Tcurborder, Tedge)) {
        if (safe.top == false)
            return 3;
        else if (safe.top == 1)
            T_CollisionState = 1;
        else return 2;
    }
    Tedge.bottom = barrier.bottom;

    Tedge.top = Tedge.bottom;
    if (Overlap(Tcurborder, Tedge)) {
        if (safe.bottom == false)
            return 3;
        else if (safe.bottom < 0)
            return 2;
    }
    Tedge.top = barrier.top;

    Tedge.top++; Tedge.bottom--;
    Tedge.right = Tedge.left;
    if (Overlap(Tcurborder, Tedge)) {
        if (safe.left == false)
            return 3;
        else if (safe.left < 0)
            return 2;
    }
    Tedge.right = barrier.right;

    Tedge.left = Tedge.right;
    if (Overlap(Tcurborder, Tedge)) {
        if (safe.right == false)
            return 3;
        else if (safe.right < 0)
            return 2;
    }
    Tedge.left = barrier.left;
    return T_CollisionState;
}

int CPlayer::Collision(RECT* barrier, RECT* safe, int BarrierNumber)
{
    // this funcion is used to find out whether the collision occurs
    // 0 for collision, 1 for airhiking, 2 for safe collision occurs on the top
    RECT Tcurborder; GetCurBorder(&Tcurborder);

    if (Tcurborder.bottom > 600) return 0;

    int T_EndCollisionState = 1;
    for (int i = 0; i < BarrierNumber; ++i)
    {
        int T_CollisionState = Collision_One(barrier[i], safe[i], Tcurborder);
        if (T_CollisionState == 3) return 0;
        if (T_CollisionState == 2) safe[i].top = 200; //reflect the information
        if (T_CollisionState == 1) T_EndCollisionState = 2;
    }
    return T_EndCollisionState;
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

    if (keyboard_state[DIK_SPACE] & 0x80) {
        if (!m_JumpClick) {
            Jump();
            m_Player.Move();
            m_JumpClick = true;
        }
    }
    else m_JumpClick = false;

    if (keyboard_state[DIK_DOWN] & 0x80)
        Slide();

    return true;
}

bool CPlayer::Jump() {
    if (m_ePlayerState == ePlayerStateAirHiking) return 0;
    if (m_ePlayerState == ePlayerStateJumping) {
        if (!g_IsSilent) {
            if (m_PlayerSkin == 2)
                mciSendString("play .\\sound\\GamePlaying\\action\\second_jump_girl.wav", NULL, 0, NULL);
            else
                mciSendString("play .\\sound\\GamePlaying\\action\\second_jump_boy.wav", NULL, 0, NULL);
        }
        m_ePlayerState = ePlayerStateAirHiking;
        MoveAirHiking();
    }
    else {
        if (!g_IsSilent) {
            mciSendString("play .\\sound\\GamePlaying\\action\\jump.wav", NULL, 0, NULL);
        }
        m_ePlayerState = ePlayerStateJumping;
        MoveJumping();
    }
    return 1;
}

bool CPlayer::Slide()
{
    if (m_ePlayerState == ePlayerStateJumping || m_ePlayerState == ePlayerStateAirHiking) {
        if (m_UnderBorder == 600) return 0;
        if(!g_IsSilent)
            mciSendString("play .\\sound\\GamePlaying\\action\\land.wav", NULL, 0, NULL);
        m_Player.y = m_UnderBorder - PlayerHeight;
        m_ePlayerState = ePlayerStateSliding;
        MoveSliding();
    }
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