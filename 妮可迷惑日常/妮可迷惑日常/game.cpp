#include"game.h"
extern CButton button[20];
extern CCheckBox checkbox[20];
extern CInputBox inputbox[5];
extern CAchievement achievement[50];
extern CStaticObstacle staticobstacle[20];
extern CMovingObstacle movingobstacle[20];
EXTERN_INPUT_DATA()
Clock gametime;
Save g_local, g_last;
bool g_IsSilent = false;
std::string g_filename[15] = {
    "field","heat","prog_design","exercise","math","complex","force", "experiment",
    "biology", "cell","statistics", "chemistry","quantum"
};
#define SOMATH 5
#define SOFORCE 7
#define SOBIOLOGY 9
inline void CGame::SetGameState(CGame::EGameState eGameStateCurrent)
{
    m_eGameState = eGameStateCurrent;
}

inline void CGame::SetWindowHandle(HWND hwnd)
{
    m_hWnd = hwnd;
}
void CGame::GameInit()
{
    SetGameState(PREFACE);
    //SetGameState(STATISTICSCOUNT);
    //load buttons in main menu
    button[ISINGLE_MODE].Create(ISINGLE_MODE, 271, 63, 250, 160, "button1");
    button[IMULTIMODE].Create(IMULTIMODE, 271, 63, 250, 243, "button2");
    button[ISETTINGS].Create(ISETTINGS, 271, 63, 250, 326, "button3");
    button[IHELP].Create(IHELP, 271, 63, 250, 409, "button4");
    button[ILOG].Create(ILOG, 150, 62, 50, 500, "log");
    button[IREGISTRY].Create(IREGISTRY, 150, 62, 220, 500, "registry");
    //load return button
    button[IRETURN].Create(IRETURN, 80, 80, 0, 0, "return");
    //load skin button
    button[IPETERSKIN].Create(IPETERSKIN, 150, 200, 100, 120, "Peter");
    button[ISATASKIN].Create(ISATASKIN, 150, 200, 300, 120, "Sata");
    button[ITINGTINGSKIN].Create(ITINGTINGSKIN, 150, 200, 500, 120, "TingTing");
    button[IMBGSKIN].Create(IMBGSKIN, 150, 200, 100, 340, "MBG");
    button[ICWKSKIN].Create(ICWKSKIN, 150, 200, 300, 340, "CWK");
    button[IOK_SELECTHARDSHIP].Create(IOK_SELECTHARDSHIP, 152, 63, 590, 500, "OK_SelectHardship");
    button[IACHIEVEMENT].Create(IACHIEVEMENT, 128, 256, 600, 350, "achievement");
    button[IOK_MATH_TEST].Create(IOK_MATH_TEST, 152, 63, 590, 500, "OK_SelectHardship");

    checkbox[JSILENCE].Create(JSILENCE, JSILENCE_WIDTH, JSILENCE_HEIGHT,
        JSILENCE_X, JSILENCE_Y, "silence", CSTATEOFF);

    inputbox[IHARDSHIPBOX].Create(IHARDSHIPBOX, 250, 25, 80, 250, false);
    inputbox[IMATH_ANSBOX].Create(IMATH_ANSBOX, 250, 25, 80, 370, false);

    achievement[ACJUANWANG].Create(50, 100, "the king of JUAN", "You are really 'JUAN'!");
    achievement[ACSTOP].Create(50, 160, "don't force to JUAN", "I said stop stop!");
    achievement[ACWARMMING].Create(50, 220, "warmming up", "You have mastered a little!");
    achievement[ACMASTER].Create(50, 280, "master", "You are so huge!");
    achievement[ACNICO].Create(50, 340, "Meeting Nico", "NicoNico!");
    achievement[ACCONFUSE].Create(50, 400, "Getting confused", "NicoNiNico!");
    achievement[ACNICOCONFUSE].Create(50, 460, "Nico gets confused", "NicoNicoNi!");
    achievement[ACMATHKING].Create(50, 520, "the king of math", "You are so huge in math!");
    achievement[ACMATHFAIL].Create(420, 100, "math failed", "You have failed in math!");
    achievement[ACFORCEKING].Create(420, 160, "the king of force", "You are so huge in force!");
    achievement[ACFORCEFAIL].Create(420, 220, "force failed", "You have failed in force!");
    achievement[ACBIOLOGYKING].Create(420, 280, "the king of biology", "You are so huge in biology!");
    achievement[ACBIOLOGYFAIL].Create(420, 340, "biology failed", "You have failed in biology!");

    staticobstacle[SOFIELD].Create(1, 0, 0, 0, 0, "field");
    for(int i = 1;i <= 7; ++i)
        staticobstacle[i].Create(-1, -1, -1, -1, 20, g_filename[i]); //danger
    for(int i = 8;i <= 12; ++i)
        staticobstacle[i].Create(1, -1, -1, -1, 20, g_filename[i]); //undanger
    staticobstacle[SOJUAN].Create(-1, -1, -1, -1, -10, "juan");
    staticobstacle[SOYINGJUAN].Create(-1, -1, -1, -1, -10, "yingjuan");


    movingobstacle[MOCONFUSE].Create(-1, -1, -1, -1, 20, 5, "confuse");
    movingobstacle[MONICO].Create(-1, -1, -1, -1, 10, 5, "nico");

    hp.Load_Frame("HP");

    DInput_Init();
    DInput_Init_Keyboard();
    DInput_Init_Mouse();
    m_connected = 0;
    m_loggedin = 0;

    CSaveTools::LoadSave(g_local);
    g_last = g_local;
}
void CGame::GameMain()
{
    gametime.Start_Clock();
    switch (m_eGameState)
    {
    case PREFACE:
        Preface();
        break;
    case MAINMENU:
        ShowMenu();
        break;
    case SELECT_SKIN:
        SelectSkin();
        break;
    case SETTINGS:
        Settings();
        break;
    case HELP:
        Help();
        break;
    case ACHIEVEMENT:
        Achievement();
        break;
    case LOG:
        ConnectToServ();
        break;
    case REGISTRY:
        Reg();
        break;
    case SELECT_HARDNESS:
        SelectHardship();
        break;
    case WAITOTHERS:
        WaitOthers();
        break;
    case PRELUDE:
        Prelude();
        break;
    case SINGLEPLAY:
        SinglePlayer();
        break;
    case SINGLEFAILURE:
        SingleFailure();
        break;
    case SINGLESUCCESS:
        SingleSuccess();
        break;
    case STATISTICSCOUNT:
        Statisticscount();
        break;
    default:
        break;
    }
    HWND hw;
    hw = GetForegroundWindow();
    if (hw == main_window_handle)
    {
        GetCurMsg();//include mouse and keyboard.understand "cur" as "current",not "cursor"
        ProcessButtonMsg();
        ProcessCheckBoxMsg();
        ProcessKeyMsg();
    }

    gametime.Wait_Clock(14);
}
void CGame::Preface()
{
    static BOB pref[2];
    static bool inited = false;
    if (!inited)
    {
        for (int i = 0; i < 2; ++i)
            pref[i].Create(0, 0, 800, 600, 30, BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME);
        BITMAP_FILE picture1, picture2;
        for (int i = 0; i < 2; ++i) {
            std::string name1, name2;
            name1 = "Preface\\Preface" + std::to_string(i) + ".bmp";
            name2 = "Preface\\Preface" + std::to_string(i + 1) + ".bmp";
            picture1.Load_File(name1.c_str());
            picture2.Load_File(name2.c_str());
            pref[i].Load_Gradual_Frame(&picture1, &picture2, 0, 0, 10);
            pref[i].Set_Anim_Speed(3);
            picture1.Unload_File(); picture2.Unload_File();
        }
        inited = true;
        for (int i = 0; i < 5; ++i) pref[0].Animate();			/***************************/
    }
    Clock t_clock;
    t_clock.Start_Clock();

    static int frame{ -30 };									/***************************/
    DDraw_Fill_Surface(lpddsback, 0);
    static int counter = 0;
    if (frame == 76)
    {
        pref[1].Draw(lpddsback);
        if (counter >= 100) frame++;
        counter++;
        t_clock.Wait_Clock(30);
        return;
    }

    if (frame++ < 0) {
        pref[0].Draw(lpddsback);
    }
    else if (frame < 76) {
        pref[0].Animate();
        pref[0].Draw(lpddsback);
    }
    else if (frame < 150) {
        pref[1].Animate();
        pref[1].Draw(lpddsback);
    }
    else
    {
        pref[1].Draw(lpddsback);
        if (counter >= 170)
            m_eGameState = MAINMENU;
        counter++;
    }
    if (keyboard_state[DIK_LCONTROL] & 0x80)
        counter = frame = 200;
    t_clock.Wait_Clock(30);
}
void CGame::ConnectToServ()
{
    MessageBox(NULL, "Function not defined", "Attention", MB_OK);
    return;
}
void CGame::GetCurMsg()
{
    GetCursorPos(&pos);
    ScreenToClient(main_window_handle, &pos);
    DInput_Read_Mouse();
    DInput_Read_Keyboard();
    return;
}

void CGame::ProcessButtonMsg()
{
    switch (m_eGameState)
    {
    case MAINMENU:
        for (int i = ISINGLE_MODE; i <= IREGISTRY; i++)
        {
            button[i].Check();
            if (button[i].m_state == BSTATEUP)
            {
                if (!g_IsSilent) mciSendString("play .\\sound\\click\\0.wav", NULL, 0, NULL);
                switch (i)
                {
                case ISINGLE_MODE:m_IsSingle = true; m_eGameState = SELECT_SKIN; break;
                case IMULTIMODE:m_IsSingle = false; m_eGameState = SELECT_SKIN; break;
                case ISETTINGS:m_eGameState = SETTINGS; break;
                case IHELP:m_eGameState = HELP; break;
                case ILOG:m_eGameState = LOG; break;
                case IACHIEVEMENT:m_eGameState = ACHIEVEMENT; break;
                case IREGISTRY:m_eGameState = REGISTRY; break;
                default:
                    break;
                }
                button[i].m_state = BSTATENORMAL;
            }
        }
        break;
    case SETTINGS:
        button[IRETURN].Check();
        if (button[IRETURN].m_state == BSTATEUP) {
            if (!g_IsSilent) mciSendString("play .\\sound\\click\\0.wav", NULL, 0, NULL);
            m_eGameState = MAINMENU;
            button[IRETURN].m_state = BSTATENORMAL;
        }
        break;
    case HELP:
        button[IRETURN].Check();
        if (button[IRETURN].m_state == BSTATEUP) {
            if (!g_IsSilent) mciSendString("play .\\sound\\click\\0.wav", NULL, 0, NULL);
            m_eGameState = MAINMENU;
            button[IRETURN].m_state = BSTATENORMAL;
        }
        break;
    case ACHIEVEMENT:
        button[IRETURN].Check();
        if (button[IRETURN].m_state == BSTATEUP) {
            if (!g_IsSilent) mciSendString("play .\\sound\\click\\0.wav", NULL, 0, NULL);
            m_eGameState = MAINMENU;
            button[IRETURN].m_state = BSTATENORMAL;
        }
        break;
    case STATISTICSCOUNT:
        button[IRETURN].Check();
        if (button[IRETURN].m_state == BSTATEUP) {
            if (!g_IsSilent) mciSendString("play .\\sound\\click\\0.wav", NULL, 0, NULL);
            m_eGameState = MAINMENU;
            button[IRETURN].m_state = BSTATENORMAL;
        }
        break;
    case SELECT_SKIN:
        for (int i = IPETERSKIN; i <= ICWKSKIN; i++)
        {
            button[i].Check();
            if (button[i].m_state == BSTATEUP) {
                if (!g_IsSilent) mciSendString("play .\\sound\\click\\0.wav", NULL, 0, NULL);
                if (!g_IsSilent && i == IMBGSKIN) mciSendString("play .\\sound\\click\\mbg.mp3", NULL, 0, NULL);
                int T_PlayerSkin = i - IPETERSKIN;
                m_player.Create(SkinFrameNumber[T_PlayerSkin]);
                m_player.LoadSkinFrame(T_PlayerSkin, SkinRunningNumber[T_PlayerSkin], SkinAirHikingNumber[T_PlayerSkin]);
                if (m_IsSingle) SetGameState(SELECT_HARDNESS);
                else SetGameState(WAITOTHERS);
                button[i].m_state = BSTATENORMAL;
            }
        }
        break;
    case SELECT_HARDNESS:
        button[IOK_SELECTHARDSHIP].Check();
        if (button[IOK_SELECTHARDSHIP].m_state == BSTATEUP)
        {
            if (!g_IsSilent) mciSendString("play .\\sound\\click\\0.wav", NULL, 0, NULL);
            m_hardness = atoi(inputbox[IHARDSHIPBOX].m_input);
            m_map.Create(m_hardness / 10 + 1, m_hardness, 400, 600);
            SetGameState(PRELUDE);
            button[IOK_SELECTHARDSHIP].m_state = BSTATENORMAL;
            mciSendString("stop .\\sound\\bgmusic\\1.mp3", NULL, 0, NULL);
            mciSendString("stop .\\sound\\bgmusic\\0.mp3", NULL, 0, NULL);
        }
        break;
    default:
        break;
    }
    return;
}

void CGame::ProcessCheckBoxMsg()
{
    switch (m_eGameState)
    {
    case SETTINGS:
        checkbox[JSILENCE].Check();
        g_IsSilent = !checkbox[JSILENCE].m_state;
    default:
        break;
    }
    return;
}

void CGame::ProcessKeyMsg()
{
    switch (m_eGameState)
    {
    case SELECT_HARDNESS:
        inputbox[IHARDSHIPBOX].Check();
        break;
    default:
        break;
    }
    return;
}
int firstopen = true, firststop = true;
void CGame::ShowMenu()
{
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(".\\background\\MainMenu.bmp");
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    for (int i = ISINGLE_MODE; i <= IREGISTRY; i++) button[i].Draw();
    button[IACHIEVEMENT].Draw();
    if (g_IsSilent && firststop) {
        mciSendString("stop .\\sound\\bgmusic\\1.mp3", NULL, 0, NULL);
        mciSendString("stop .\\sound\\bgmusic\\0.mp3", NULL, 0, NULL);
        firstopen = true; firststop = false;
    }
    if (!g_IsSilent && firstopen) {
        int Tcur = rand() % 2;
        mciSendString(("play .\\sound\\bgmusic\\" + std::to_string(Tcur) + ".mp3 repeat").c_str(), 
            NULL, 0, NULL);
        firstopen = false; firststop = true;

    }
    return;
}

void RENEWSAVE() 
{
    if (g_local.iStObstacleFailure[SOJUAN] >= 100)
        g_local.bAchievements[ACJUANWANG] = true;
    if (g_local.iStObstacleFailure[SOYINGJUAN] >= 50)
        g_local.bAchievements[ACSTOP] = true;
    if (g_local.MapLength >= 50000)
        g_local.bAchievements[ACWARMMING] = true;
    if (g_local.MapLength >= 500000)
        g_local.bAchievements[ACMASTER] = true;
    if (g_local.iStObstacleFailure[MONICO] >= 30)
        g_local.bAchievements[ACNICO] = true;
    if (g_local.iStObstacleFailure[MOCONFUSE] >= 30)
        g_local.bAchievements[ACCONFUSE] = true;
    if (g_local.iStObstacleFailure[MONICO] - g_last.iStObstacleFailure[MONICO] >= 10 &&
        g_local.iStObstacleFailure[MOCONFUSE] - g_last.iStObstacleFailure[MOCONFUSE] >= 10)
        g_local.bAchievements[ACNICOCONFUSE] = true;
    if (g_local.iStObstacleSuccess[SOMATH] >= 10)
        g_local.bAchievements[ACMATHKING] = true;
    if (g_local.iStObstacleFailure[SOMATH] >= 10)
        g_local.bAchievements[ACMATHFAIL] = true;
    if (g_local.iStObstacleSuccess[SOFORCE] >= 10)
        g_local.bAchievements[ACFORCEKING] = true;
    if (g_local.iStObstacleFailure[SOFORCE] >= 10)
        g_local.bAchievements[ACFORCEFAIL] = true;
    if (g_local.iStObstacleSuccess[SOBIOLOGY] >= 10)
        g_local.bAchievements[ACBIOLOGYKING] = true;
    if (g_local.iStObstacleFailure[SOBIOLOGY] >= 10)
        g_local.bAchievements[ACBIOLOGYFAIL] = true;
    CSaveTools::SaveDirectly(g_local);
}

void CGame::SinglePlayer()
{
    static int first = 1, T_cur, T_background = rand() % 4;
    if (first && !g_IsSilent) {
        T_cur = rand() % 3;
        mciSendString(
            ("play .\\sound\\GamePlaying\\bgsound\\" + std::to_string(T_cur) + ".mp3 repeat").c_str(),
            NULL, 0, NULL);
        first = false;
    }
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File((".\\background\\PlayingBackground" + std::to_string(T_background) + ".bmp").c_str()); 
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    if (!m_map.MoveNext())
    {
        g_local.MapLength += m_map.m_Length;
        RENEWSAVE();
        mciSendString(
            ("stop .\\sound\\GamePlaying\\bgsound\\" + std::to_string(T_cur) + ".mp3").c_str(),
            NULL, 0, NULL);
        first = true;
        m_state = true;
        SetGameState(SINGLESUCCESS);
        return;
    }
    m_map.Draw(m_player.m_HP / 10);
    m_player.GetUnder(m_map.m_CurBarrier, m_map.m_CurSafe, m_map.m_BarrierNumber);
    m_player.MoveNext();
    m_player.Draw();
    DDraw_Flip();
    if (!m_player.LogicRun(m_map.m_CurBarrier, m_map.m_CurSafe, m_map.m_BarrierNumber)) 
        SetGameState(SINGLEFAILURE);
    int THPReduce = m_map.Fix();
    m_player.m_HP -= THPReduce;
    if (THPReduce > 0) {
        if (!g_IsSilent)
            mciSendString("play .\\sound\\GamePlaying\\hitted\\0.wav", NULL, 0, NULL);
    }
    if (m_player.m_HP < 0)
        SetGameState(SINGLEFAILURE);
    if (m_player.m_HP > 150)
        SetGameState(SINGLEFAILURE);
    if (m_eGameState != SINGLEPLAY) {
        m_state = false;
        g_local.MapLength += m_map.m_Right;
        RENEWSAVE();
        mciSendString(
            ("stop .\\sound\\GamePlaying\\bgsound\\" + std::to_string(T_cur) + ".mp3").c_str(),
            NULL, 0, NULL);
        first = true;
    }
    return;
}

void CGame::SingleFailure()
{
    static bool next_available = true;//prevent from overacting to button messages
    static int first = 1;//first time to play music
    srand((unsigned)time(NULL));
    if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80 && next_available)
    {
        m_postlude_frame++;
        next_available = false;
    }
    if (!(mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80)) next_available = true;
    if (m_postlude_frame >= fail_frame_number[m_prelude_ID])
    {
        mciSendString("stop .\\sound\\Postlude\\failure\\math_test.wav", NULL, 0, NULL);
        mciSendString("stop .\\sound\\Postlude\\failure\\confession.mp3", NULL, 0, NULL);
        m_postlude_frame = 0;
        first = 1;
        m_prelude_ID = -1;
        m_player.m_Player.Destroy();
        if (!g_IsSilent) {
            if (rand() % 2 == 0)  mciSendString("play .\\sound\\bgmusic\\0.mp3 repeat", NULL, 0, NULL);
            else mciSendString("play .\\sound\\bgmusic\\1.mp3 repeat", NULL, 0, NULL);
        }
        SetGameState(STATISTICSCOUNT);
        return;
    }
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    char path[50];
    strcpy(path, ".\\Postlude\\");
    switch (m_prelude_ID)
    {
    case ISIGN_NAME:
        strcat(path, "sign_name\\fail\\");
        break;
    case IMATH_TEST:
        if (m_postlude_frame == 0 && first && !g_IsSilent)
        {
            mciSendString("play .\\sound\\Postlude\\failure\\math_test.wav", NULL, 0, NULL);
            first = 0;
        }
        strcat(path, "math_test\\fail\\");
        break;
    case ICONFESSION:
        if (m_postlude_frame == 4 && first && !g_IsSilent)
        {
            mciSendString("play .\\sound\\Postlude\\failure\\confession.mp3 repeat", NULL, 0, NULL);
            first = 0;
        }
        strcat(path, "confession\\fail\\");
        break;
    case I2048:
        strcat(path, "2048\\fail\\");
        break;
    case ICARD:
        strcat(path, "card\\fail\\");
        break;
    default:
        MessageBox(NULL, "switch in Postlude() of game.cpp failed!", "Attention", MB_OK);
        break;
        return;
    }
    char tmp[2];
    _itoa(m_postlude_frame, tmp, 10);
    strcat(path, tmp);
    strcat(path, ".bmp");
    bitmap->Load_File(path);
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    return;
}

void CGame::SingleSuccess()
{
    switch (m_prelude_ID)
    {
    case I2048:
        play_2048();
        break;
    case IMATH_TEST:
        play_math();
        break;
    case ICONFESSION:
        confess();
        break;
    case ISIGN_NAME:
        sign_name();
        break;
    case ICARD:
        play_card();
        break;
    default:
        break;
    }
}

void CGame::MultiPlayer()
{
    MessageBox(NULL, "Function not defined", "Attention", MB_OK);
    return;
}

void CGame::Help()
{
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(".\\background\\Help.bmp");
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    button[IRETURN].Draw();
    return;
}


void CGame::Achievement()
{
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(".\\background\\achievements.bmp");
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    button[IRETURN].Draw();

    return;
}

void CGame::Statisticscount() {
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    if(m_state == true)
        bitmap->Load_File(".\\background\\Success.bmp");
    else
        bitmap->Load_File(".\\background\\GameOver.bmp");
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    button[IRETURN].Draw();
    RECT border;
    border.top = 150;
    border.bottom = 550;
    border.left = 50;
    border.right = 350;
    lpddsback->Blt(&border, button[m_player.m_PlayerSkin + IPETERSKIN].m_ButtonSur[0], 
        NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
    

    HDC hdc;
    lpddsback->GetDC(&hdc);
    HFONT hf;
    LOGFONT lf;
    lf.lfHeight = 40;
    lf.lfWidth = 20;
    lf.lfEscapement = 0;
    lf.lfUnderline = false;
    lf.lfStrikeOut = false;
    lf.lfOrientation = 0;
    lf.lfWeight = 500;
    lf.lfItalic = false;
    lf.lfCharSet = 0;
    lf.lfQuality = 0;
    lf.lfClipPrecision = 0;
    lf.lfPitchAndFamily = 0;
    lf.lfOutPrecision = 0;
    strcpy(lf.lfFaceName, "Comic Sans");
    hf = CreateFontIndirect(&lf);
    SelectObject(hdc, hf);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));
    std::string Tcur = "You Have Run:"; 
    TextOut(hdc, 400, 200, Tcur.c_str(), Tcur.size());
    Tcur = std::to_string(m_map.m_Right / 50) + "m!";;
    LOGFONT nlf = lf;
    nlf.lfWeight = 200; nlf.lfHeight = 80;
    hf = CreateFontIndirect(&nlf);
    SelectObject(hdc, hf);
    SetTextColor(hdc, RGB(255, 0, 0));
    TextOut(hdc, 400, 250, Tcur.c_str(), Tcur.size());
    lpddsback->ReleaseDC(hdc);
    bool flag = false;
    for (int i = 0; i <= 12; ++i)
        if (g_local.bAchievements[i] != g_last.bAchievements[i])
            flag = true;
    if (flag) {
        bitmap->Load_File(".\\GUI\\achievement\\newachievements.bmp");
        DDraw_Draw_Bitmap(bitmap, lpddsback, { 400, 400});
        bitmap->Unload_File();
    }
}

void CGame::Settings()
{
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(".\\background\\Setting.bmp"); //wait for load
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    checkbox[JSILENCE].Draw();
    button[IRETURN].Draw();
    if (g_IsSilent && firststop) {
        mciSendString("stop .\\sound\\bgmusic\\1.mp3", NULL, 0, NULL);
        mciSendString("stop .\\sound\\bgmusic\\0.mp3", NULL, 0, NULL);
        firstopen = true; firststop = false;
    }
    if (!g_IsSilent && firstopen) {
        int Tcur = rand() % 2;
        mciSendString(("play .\\sound\\bgmusic\\" + std::to_string(Tcur) + ".mp3 repeat").c_str(),
            NULL, 0, NULL);
        firstopen = false; firststop = true;

    }
    return;
}

void CGame::SelectSkin()
{
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(".\\background\\SelectSkin.bmp"); //wait for load
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    for (int i = IPETERSKIN; i <= ICWKSKIN; i++)
    {
        button[i].Draw();
    }
    return;
}

void CGame::SelectHardship()
{
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(".\\background\\SelectHardship.bmp"); //wait for load
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    button[IOK_SELECTHARDSHIP].Draw();
    inputbox[IHARDSHIPBOX].Draw();
    return;
}

void CGame::Login()
{
    MessageBox(NULL, "Function not defined", "Attention", MB_OK);
    return;
}

void CGame::Reg()
{
    MessageBox(NULL, "Function not defined", "Attention", MB_OK);
    return;
}

void CGame::Prelude()
{
    static bool next_available = true;//prevent from overacting to button messages
    srand((unsigned)time(NULL));
    if (m_prelude_ID == -1)
    {
        m_prelude_ID = rand() % 5;
        m_prelude_frame = 0;
    }
    if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80 && next_available)
    {
        m_prelude_frame++;
        next_available = false;
    }
    if (!(mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80)) next_available = true;
    if (m_prelude_frame >= frame_number[m_prelude_ID])
    {
        SetGameState(SINGLE_PLAYER_BEGIN);
        return;
    }
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    char path[50];
    strcpy(path, ".\\Prelude\\");
    switch (m_prelude_ID)
    {
    case ISIGN_NAME:
        strcat(path, "sign_name\\");
        break;
    case IMATH_TEST:
        strcat(path, "math_test\\");
        break;
    case ICONFESSION:
        strcat(path, "confession\\");
        break;
    case I2048:
        strcat(path, "2048\\");
        break;
    case ICARD:
        strcat(path, "card\\");
        break;
    default:
        MessageBox(NULL, "switch in Prelude() of game.cpp failed!", "Attention", MB_OK);
        return;
    }
    char tmp[2];
    _itoa(m_prelude_frame, tmp, 10);
    strcat(path, tmp);
    strcat(path, ".bmp");
    bitmap->Load_File(path);
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    return;
}

void CGame::WaitOthers()
{
    MessageBox(NULL, "Function WaitOthers() not developed!", "Attention", MB_OK);
    return;
}
CGame::~CGame()
{
    DInput_Release_Keyboard();
    DInput_Read_Mouse();
    DInput_Shutdown();
    DDraw_Shutdown();
}


void CGame::play_2048()
{
    static int next_move_available = 1;
    static int first = 1;
    static int fail = 0, win = 0;
    static int state[4][4] = {
        1,1,1,1,
        1,1,1,1,
        1,1,1,1,
        1,1,1,1
    };
    if (fail == 1)
    {
        if (fail_2048() == 0)
        {
            fail = 0;
            first = 1;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    state[i][j] = 1;
                }
            }
        }
        return;
    }
    if (win == 1)
    {
        if (success_2048() == 0)
        {
            win = 0;
            first = 1;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    state[i][j] = 1;
                }
            }
        }
        return;
    }
    if (first)
    {
        state[rand() % 4][rand() % 4] = 2;
        state[rand() % 4][rand() % 4] = 2;
        first = 0;
    }
    POINT position[4][4] = {
        121,20,265,20,407,20,551,20,
        121,164,265,164,407,164,551,164,
        121,303,265,303,407,303,551,303,
        121,450,265,450,407,450,551,450
    };
    bool finish = false, moved = false;
    if (keyboard_state[DIK_UPARROW] & 0x80 && next_move_available)
    {
        next_move_available = false;
        while (!finish)
        {
            finish = true;
            for (int j = 0; j < 4; j++)
            {
                for (int i = 1; i < 4; i++)
                {
                    if (state[i][j] != 1 && state[i - 1][j] == 1)
                    {
                        finish = false;
                        moved = true;
                        state[i - 1][j] = state[i][j];
                        state[i][j] = 1;
                    }
                }
            }
        }
        for (int j = 0; j < 4; j++)
        {
            for (int i = 1; i < 4; i++)
            {
                if (state[i][j] != 1 && state[i - 1][j] == state[i][j])
                {
                    moved = 1;
                    state[i - 1][j] *= 2;
                    state[i][j] = 1;
                }
            }
        }
        finish = false;
        while (!finish)
        {
            finish = true;
            for (int j = 0; j < 4; j++)
            {
                for (int i = 1; i < 4; i++)
                {
                    if (state[i][j] != 1 && state[i - 1][j] == 1)
                    {
                        finish = false;
                        moved = true;
                        state[i - 1][j] = state[i][j];
                        state[i][j] = 1;
                    }
                }
            }
        }
    }
    if (keyboard_state[DIK_DOWNARROW] & 0x80 && next_move_available)
    {
        next_move_available = false;
        while (!finish)
        {
            finish = true;
            for (int j = 0; j < 4; j++)
            {
                for (int i = 2; i >= 0; i--)
                {
                    if (state[i][j] != 1 && state[i + 1][j] == 1)
                    {
                        finish = false;
                        moved = true;
                        state[i + 1][j] = state[i][j];
                        state[i][j] = 1;
                    }
                }
            }
        }
        for (int j = 0; j < 4; j++)
        {
            for (int i = 2; i >= 0; i--)
            {
                if (state[i][j] != 1 && state[i + 1][j] == state[i][j])
                {
                    moved = 1;
                    state[i + 1][j] *= 2;
                    state[i][j] = 1;
                }
            }
        }
        finish = false;
        while (!finish)
        {
            finish = true;
            for (int j = 0; j < 4; j++)
            {
                for (int i = 2; i >= 0; i--)
                {
                    if (state[i][j] != 1 && state[i + 1][j] == 1)
                    {
                        finish = false;
                        moved = true;
                        state[i + 1][j] = state[i][j];
                        state[i][j] = 1;
                    }
                }
            }
        }
    }
    if (keyboard_state[DIK_LEFTARROW] & 0x80 && next_move_available)
    {
        next_move_available = false;
        while (!finish)
        {
            finish = true;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 1; j < 4; j++)
                {
                    if (state[i][j] != 1 && state[i][j - 1] == 1)
                    {
                        finish = false;
                        moved = true;
                        state[i][j - 1] = state[i][j];
                        state[i][j] = 1;
                    }
                }
            }
        }
        for (int i = 0; i < 4; i++)
        {
            for (int j = 1; j < 4; j++)
            {
                if (state[i][j] != 1 && state[i][j - 1] == state[i][j])
                {
                    moved = 1;
                    state[i][j - 1] *= 2;
                    state[i][j] = 1;
                }
            }
        }
        finish = false;
        while (!finish)
        {
            finish = true;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 1; j < 4; j++)
                {
                    if (state[i][j] != 1 && state[i][j - 1] == 1)
                    {
                        finish = false;
                        moved = true;
                        state[i][j - 1] = state[i][j];
                        state[i][j] = 1;
                    }
                }
            }
        }
    }
    if (keyboard_state[DIK_RIGHTARROW] & 0x80 && next_move_available)
    {
        next_move_available = false;
        while (!finish)
        {
            finish = true;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 2; j >= 0; j--)
                {
                    if (state[i][j] != 1 && state[i][j + 1] == 1)
                    {
                        finish = false;
                        moved = true;
                        state[i][j + 1] = state[i][j];
                        state[i][j] = 1;
                    }
                }
            }
        }
        for (int i = 0; i < 4; i++)
        {
            for (int j = 2; j >= 0; j--)
            {
                if (state[i][j] != 1 && state[i][j + 1] == state[i][j])
                {
                    moved = 1;
                    state[i][j + 1] *= 2;
                    state[i][j] = 1;
                }
            }
        }
        finish = false;
        while (!finish)
        {
            finish = true;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 2; j >= 0; j--)
                {
                    if (state[i][j] != 1 && state[i][j + 1] == 1)
                    {
                        finish = false;
                        moved = true;
                        state[i][j + 1] = state[i][j];
                        state[i][j] = 1;
                    }
                }
            }
        }
    }
    if (!(keyboard_state[DIK_RIGHTARROW] & 0x80) && !(keyboard_state[DIK_LEFTARROW] & 0x80) && !(keyboard_state[DIK_UPARROW] & 0x80) && !(keyboard_state[DIK_DOWNARROW] & 0x80)) next_move_available = true;
    if (moved)
    {
        int x, y;
        do
        {
            x = rand() % 4;
            y = rand() % 4;
        } while (state[y][x] != 1);
        state[y][x] = 2;
    }
    fail = 1;
    win = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (state[i][j] == state[i + 1][j] || state[i][j] == state[i][j + 1] || state[i][j] == 1 || state[i + 1][j] == 1 || state[i][j + 1] == 1) fail = 0;
            if (state[i][j] == 2048 || state[i + 1][j] == 2048 || state[i][j + 1] == 2048) win = 1;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        if (state[i][3] == state[i + 1][3] || state[3][i] == state[3][i + 1]) fail = 0;
    }
    if (state[3][3] == 1) fail = 0;
    if (state[3][3] == 2048) win = 1;
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(".\\Postlude\\2048\\OnPlaying\\0.bmp");
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    //draw the board
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (state[i][j] != 1)
            {
                std::string address;
                address = ".\\Postlude\\2048\\OnPlaying\\";
                address = address + std::to_string(state[i][j]) + ".bmp";
                bitmap->Load_File(address.c_str());
                DDraw_Draw_Bitmap(bitmap, lpddsback, position[i][j]);
                bitmap->Unload_File();
            }
        }
    }
}
void CGame::play_math()
{
    double ans[16] = { 0.5,1,1,2,0.667,0.5,325,2,2,1,0,1,0.5,0,2,1 };
    static int problem = rand() % 16, success = 0;
    if (success == 1)
    {
        if (success_math() == 0)
        {
            problem = rand() % 16;
            success = 0;
        }
        return;
    }
    if (success == -1)
    {
        if (fail_math() == 0)
        {
            problem = rand() % 16;
            success = 0;
        }
        return;
    }
    std::string address = ".\\Postlude\\math_test\\OnPlaying\\" + std::to_string(problem) + ".bmp";
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(address.c_str());
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    inputbox[IMATH_ANSBOX].Check();
    button[IOK_MATH_TEST].Check();
    inputbox[IMATH_ANSBOX].Draw();
    button[IOK_MATH_TEST].Draw();
    if (button[IOK_MATH_TEST].m_state == BSTATEUP)
    {
        char tmp[50]; double input;
        strcpy(tmp, inputbox[IMATH_ANSBOX].m_input);
        if (tmp[strlen(tmp) - 1] == '|') tmp[strlen(tmp) - 1] = 0;
        input = atof(tmp);
        if (input - ans[problem] < 0.01 && ans[problem] - input < 0.01) success = 1;
        else success = -1;
        button[IOK_MATH_TEST].m_state = BSTATENORMAL;
    }
    return;
}
void CGame::play_card()
{

    static int position[3] = { 0,1,2 };//分别为A，K，Q
    static int order = 0;
    POINT coor[3] = { 250,300,370,300,490,300 };
    static POINT actual_coor[3] = { 250,300,370,300,490,300 };
    static int state[3] = { 1,1,1 };//1代表牌正面朝上
    static int success = 0;//0:在玩，1：胜，-1：负
    static int finished_move = -1;
    static int move_time = rand() % 20 + 20;
    static int first = 1;
    static clock_t clok;
    if (success == 1)
    {
        if (success_card() == 0)
        {
            position[0] = 0; position[1] = 1; position[2] = 2;
            order = 0;
            for (int i = 0; i < 3; i++)
            {
                actual_coor[i].x = coor[i].x;
                actual_coor[i].y = coor[i].y;
            }
            state[0] = state[1] = state[2] = 1;
            success = 0;
            finished_move = -1;
            move_time = rand() % 20 + 20;
            first = 1;
        }
        return;
    }
    if (success == -1)
    {
        if (fail_card() == 0)
        {
            position[0] = 0; position[1] = 1; position[2] = 2;
            order = 0;
            for (int i = 0; i < 3; i++)
            {
                actual_coor[i].x = coor[i].x;
                actual_coor[i].y = coor[i].y;
            }
            state[0] = state[1] = state[2] = 1;
            success = 0;
            finished_move = -1;
            move_time = rand() % 20 + 10;
            first = 1;
        }
        return;
    }

    if (finished_move == 0) //move cards
    {
        if (clock() - clok < 2000) return;
        if (actual_coor[0].x == coor[position[0]].x && actual_coor[1].x == coor[position[1]].x && actual_coor[2].x == coor[position[2]].x)
        {
            order = rand() % 3;
            move_time--;
            if (!move_time)
            {
                finished_move = 1;
                return;
            }
            int tmp;
            position[0] = rand() % 3;
            do
            {
                tmp = rand() % 3;
            } while (tmp == position[0]);
            position[1] = tmp;
            position[2] = 3 - position[0] - position[1];
        }
        //move by one step
        for (int i = 0; i < 3; i++)
        {
            if (actual_coor[i].x > coor[position[i]].x)
            {
                actual_coor[i].x -= 20;
            }
            if (actual_coor[i].x < coor[position[i]].x)
            {
                actual_coor[i].x += 20;
            }
        }
    }
    else if (finished_move == 1)
    {
        if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80)
        {
            if (pos.x >= actual_coor[0].x && pos.x <= actual_coor[0].x + 105 && pos.y >= actual_coor[0].y && pos.y <= actual_coor[0].y + 150)
            {
                success = 1;
            }
            if (pos.x >= actual_coor[1].x && pos.x <= actual_coor[1].x + 105 && pos.y >= actual_coor[1].y && pos.y <= actual_coor[1].y + 150)
            {
                success = -1;
            }
            if (pos.x >= actual_coor[2].x && pos.x <= actual_coor[2].x + 105 && pos.y >= actual_coor[2].y && pos.y <= actual_coor[2].y + 150)
            {
                success = -1;
            }
        }
    }
    else //鼠标点击屏幕，开始游戏
    {
        state[0] = state[1] = state[2] = 1;
        if (first)
        {
            int tmp;
            position[0] = rand() % 3;
            do
            {
                tmp = rand() % 3;
            } while (tmp == position[0]);
            position[1] = tmp;
            position[2] = 3 - position[0] - position[1];
            first = 0;
        }
        if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80)
        {
            clok = clock();
            state[0] = state[1] = state[2] = 0;
            finished_move = 0;
        }
    }
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\background.bmp");
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    switch (order)//根据绘图顺序决定牌的前后，防止牌在最上面直接跟踪
    {
    case 0:
        if (state[0] == 1)
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\A.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[0]);
            bitmap->Unload_File();
        }
        else
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\back.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[0]);
            bitmap->Unload_File();
        }
        if (state[1] == 1)
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\K.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[1]);
            bitmap->Unload_File();
        }
        else
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\back.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[1]);
            bitmap->Unload_File();
        }
        if (state[2] == 1)
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\Q.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[2]);
            bitmap->Unload_File();
        }
        else
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\back.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[2]);
            bitmap->Unload_File();
        }
        break;
    case 1:
        if (state[1] == 1)
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\K.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[1]);
            bitmap->Unload_File();
        }
        else
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\back.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[1]);
            bitmap->Unload_File();
        }
        if (state[0] == 1)
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\A.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[0]);
            bitmap->Unload_File();
        }
        else
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\back.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[0]);
            bitmap->Unload_File();
        }
        if (state[2] == 1)
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\Q.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[2]);
            bitmap->Unload_File();
        }
        else
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\back.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[2]);
            bitmap->Unload_File();
        }
        break;
    case 2:
        if (state[2] == 1)
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\Q.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[2]);
            bitmap->Unload_File();
        }
        else
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\back.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[2]);
            bitmap->Unload_File();
        }
        if (state[1] == 1)
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\K.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[1]);
            bitmap->Unload_File();
        }
        else
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\back.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[1]);
            bitmap->Unload_File();
        }
        if (state[0] == 1)
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\A.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[0]);
            bitmap->Unload_File();
        }
        else
        {
            bitmap->Load_File(".\\Postlude\\card\\OnPlaying\\back.bmp");
            DDraw_Draw_Bitmap(bitmap, lpddsback, actual_coor[0]);
            bitmap->Unload_File();
        }
        break;
    default:
        break;
    }
    return;
}
void CGame::confess()
{
    static bool next_available = true;//prevent from overacting to button messages
    static int frame = 0, first = 1;
    if (first && frame == 1 && !g_IsSilent)
    {
        mciSendString("play .\\sound\\Postlude\\success\\confession.mp3", NULL, 0, NULL);
        first = 0;
    }
    int fnumber = 5;
    if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80 && next_available)
    {
        next_available = false;
    }
    if (!(mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80) && next_available == false)
    {
        frame++;
        next_available = true;
    }
    if (frame >= fnumber)
    {
        frame = 0;
        first = 1;
        mciSendString("stop .\\sound\\Postlude\\success\\confession.mp3", NULL, 0, NULL);
        m_prelude_ID = -1;
        m_player.m_Player.Destroy();
        SetGameState(STATISTICSCOUNT);
        if (!g_IsSilent) {
            if (rand() % 2 == 0)  mciSendString("play .\\sound\\bgmusic\\0.mp3 repeat", NULL, 0, NULL);
            else mciSendString("play .\\sound\\bgmusic\\1.mp3 repeat", NULL, 0, NULL);
        }
        return;
    }
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    char path[50];
    strcpy(path, ".\\Postlude\\confession\\success\\");
    char tmp[2];
    _itoa(frame, tmp, 10);
    strcat(path, tmp);
    strcat(path, ".bmp");
    bitmap->Load_File(path);
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    return;
}
void CGame::sign_name()
{
    static bool next_available = true;//prevent from overacting to button messages
    static int frame = 0;
    int fnumber = 2;
    if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80 && next_available)
    {
        next_available = false;
    }
    if (!(mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80) && next_available == false)
    {
        frame++;
        next_available = true;
    }
    if (frame >= fnumber)
    {
        frame = 0;
        m_prelude_ID = -1;
        m_player.m_Player.Destroy();
        SetGameState(STATISTICSCOUNT);
        if (!g_IsSilent) {
            if (rand() % 2 == 0)  mciSendString("play .\\sound\\bgmusic\\0.mp3 repeat", NULL, 0, NULL);
            else mciSendString("play .\\sound\\bgmusic\\1.mp3 repeat", NULL, 0, NULL);
        }
        return;
    }
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    char path[50];
    strcpy(path, ".\\Postlude\\sign_name\\success\\");
    char tmp[2];
    _itoa(frame, tmp, 10);
    strcat(path, tmp);
    strcat(path, ".bmp");
    bitmap->Load_File(path);
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    return;
}
int CGame::fail_2048()
{
    static int next = 0;
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE; SetGameState(STATISTICSCOUNT);
    bitmap->Load_File(".\\Postlude\\2048\\game_fail\\0.bmp");
    if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80) next = 1;
    if (next && !(mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80))
    {
        m_prelude_ID = -1;
        m_player.m_Player.Destroy();
        next = 0;
        bitmap->Unload_File();
        SetGameState(STATISTICSCOUNT);
        if (!g_IsSilent) {
            if (rand() % 2 == 0)  mciSendString("play .\\sound\\bgmusic\\0.mp3 repeat", NULL, 0, NULL);
            else mciSendString("play .\\sound\\bgmusic\\1.mp3 repeat", NULL, 0, NULL);
        }
        return 0;
    }
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    return 1;
}
int CGame::fail_math()
{
    static bool next_available = true;//prevent from overacting to button messages
    static int frame = 0;
    int fnumber = 1;
    if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80 && next_available)
    {
        next_available = false;
    }
    if (!(mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80) && next_available == false)
    {
        frame++;
        next_available = true;
    }
    if (frame >= fnumber)
    {
        frame = 0;
        m_prelude_ID = -1;
        m_player.m_Player.Destroy();
        SetGameState(STATISTICSCOUNT);
        strcpy(inputbox[IMATH_ANSBOX].m_input, "|");
        if (!g_IsSilent) {
            if (rand() % 2 == 0)  mciSendString("play .\\sound\\bgmusic\\0.mp3 repeat", NULL, 0, NULL);
            else mciSendString("play .\\sound\\bgmusic\\1.mp3 repeat", NULL, 0, NULL);
        }
        return 0;
    }
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    char path[50];
    strcpy(path, ".\\Postlude\\math_test\\game_fail\\");
    char tmp[2];
    _itoa(frame, tmp, 10);
    strcat(path, tmp);
    strcat(path, ".bmp");
    bitmap->Load_File(path);
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    return 1;
}
int CGame::fail_card()
{
    static int next1 = 0, next2 = 0;
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(".\\Postlude\\card\\game_fail\\0.bmp");
    if (!(mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80)) next1 = 1;
    if (next1 && (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80)) next2 = 1;
    if (next2 && !(mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80))
    {
        m_prelude_ID = -1;
        m_player.m_Player.Destroy();
        next1 = 0, next2 = 0;
        bitmap->Unload_File();
        SetGameState(STATISTICSCOUNT);
        if (!g_IsSilent) {
            if (rand() % 2 == 0)  mciSendString("play .\\sound\\bgmusic\\0.mp3 repeat", NULL, 0, NULL);
            else mciSendString("play .\\sound\\bgmusic\\1.mp3 repeat", NULL, 0, NULL);
        }
        return 0;
    }
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    return 1;
}
int CGame::success_2048()
{
    static int first = 1;
    if (first && !g_IsSilent)
    {
        mciSendString("play .\\sound\\Postlude\\success\\normal_success.wav", NULL, 0, NULL);
        first = 0;
    }
    static int next = 0;
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(".\\Postlude\\2048\\game_success\\0.bmp");
    if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80) next = 1;
    if (next && !(mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80))
    {
        first = 1;
        m_prelude_ID = -1;
        m_player.m_Player.Destroy();
        next = 0;
        bitmap->Unload_File();
        SetGameState(STATISTICSCOUNT);
        if (!g_IsSilent) {
            if (rand() % 2 == 0)  mciSendString("play .\\sound\\bgmusic\\0.mp3 repeat", NULL, 0, NULL);
            else mciSendString("play .\\sound\\bgmusic\\1.mp3 repeat", NULL, 0, NULL);
        }
        return 0;
    }
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    return 1;
}
int CGame::success_math()
{
    static bool next_available = true;//prevent from overacting to button messages
    static int frame = 0, first = 1;
    int fnumber = 2;
    if (first && !g_IsSilent)
    {
        mciSendString("play .\\sound\\Postlude\\success\\normal_success.wav", NULL, 0, NULL);
        first = 0;
    }
    if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80 && next_available)
    {
        next_available = false;
    }
    if (!(mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80) && next_available == false)
    {
        frame++;
        next_available = true;
    }
    if (frame >= fnumber)
    {
        frame = 0;
        first = 1;
        m_prelude_ID = -1;
        m_player.m_Player.Destroy();
        strcpy(inputbox[IMATH_ANSBOX].m_input, "|");
        SetGameState(STATISTICSCOUNT);
        if (!g_IsSilent) {
            if (rand() % 2 == 0)  mciSendString("play .\\sound\\bgmusic\\0.mp3 repeat", NULL, 0, NULL);
            else mciSendString("play .\\sound\\bgmusic\\1.mp3 repeat", NULL, 0, NULL);
        }
        return 0;
    }
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    char path[50];
    strcpy(path, ".\\Postlude\\math_test\\game_success\\");
    char tmp[2];
    _itoa(frame, tmp, 10);
    strcat(path, tmp);
    strcat(path, ".bmp");
    bitmap->Load_File(path);
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    return 1;
}
int CGame::success_card()
{
    static int first = 1;
    if (first && !g_IsSilent)
    {
        mciSendString("play .\\sound\\Postlude\\success\\card.mp3", NULL, 0, NULL);
        first = 0;
    }
    static int next1 = 0, next2 = 0;
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(".\\Postlude\\card\\game_success\\0.bmp");
    if (!(mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80)) next1 = 1;
    if (next1 && (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80)) next2 = 1;
    if (next2 && !(mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80))
    {
        first = 1;
        m_prelude_ID = -1;
        m_player.m_Player.Destroy();
        next1 = next2 = 0;
        bitmap->Unload_File();
        SetGameState(STATISTICSCOUNT);
        mciSendString("stop .\\sound\\Postlude\\success\\card.mp3", NULL, 0, NULL);
        if (!g_IsSilent) {
            if (rand() % 2 == 0)  mciSendString("play .\\sound\\bgmusic\\0.mp3 repeat", NULL, 0, NULL);
            else mciSendString("play .\\sound\\bgmusic\\1.mp3 repeat", NULL, 0, NULL);
        }
        return 0;
    }
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    return 1;
}