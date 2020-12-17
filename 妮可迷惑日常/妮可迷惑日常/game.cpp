#include"game.h"
extern CButton button[20];
extern CCheckBox checkbox[20];
extern CInputBox inputbox[5];
EXTERN_INPUT_DATA()
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

    checkbox[JSILENCE].Create(JSILENCE, JSILENCE_WIDTH, JSILENCE_HEIGHT,
        JSILENCE_X, JSILENCE_Y, "silence", CSTATEOFF);

    inputbox[IHARDSHIPBOX].Create(IHARDSHIPBOX, 250, 25, 80, 250, false);

    DInput_Init();
    DInput_Init_Keyboard();
    DInput_Init_Mouse();
    m_connected = 0;
    m_loggedin = 0;
}
void CGame::GameMain()
{
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
	default:
		break;
	}
    HWND hw;
    hw = GetForegroundWindow();
    if (hw==main_window_handle)
    {
        GetCurMsg();//include mouse and keyboard.understand "cur" as "current",not "cursor"
        ProcessButtonMsg();
        ProcessCheckBoxMsg();
        ProcessKeyMsg();
    }
    
    Sleep(30);
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
        if (counter >= 170)m_eGameState = MAINMENU;
        counter++;
    }
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
                switch (i)
                {
                case ISINGLE_MODE:m_IsSingle = true; m_eGameState = SELECT_SKIN; break;
                case IMULTIMODE:m_IsSingle = false; m_eGameState = SELECT_SKIN; break;
                case ISETTINGS:m_eGameState = SETTINGS; break;
                case IHELP:m_eGameState = HELP; break;
                case ILOG:m_eGameState = LOG; break;
                case IREGISTRY:m_eGameState = REGISTRY; break;
                default:
                    break;
                }
                button[i].m_state = BSTATENORMAL;
            }
        }
    case SETTINGS:
        button[IRETURN].Check();
        if (button[IRETURN].m_state == BSTATEUP) {
            m_eGameState = MAINMENU;
            button[IRETURN].m_state = BSTATENORMAL;
        }
        break;
    case HELP:
        button[IRETURN].Check();
        if (button[IRETURN].m_state == BSTATEUP) {
            m_eGameState = MAINMENU;
            button[IRETURN].m_state = BSTATENORMAL;
        }
    case SELECT_SKIN:
        for (int i = IPETERSKIN; i <= ICWKSKIN; i++)
        {
            button[i].Check();
            if (button[i].m_state == BSTATEUP) {
                m_player.m_PlayerSkin = i - IPETERSKIN;
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
            m_hardness = atoi(inputbox[IHARDSHIPBOX].m_input);
            SetGameState(PRELUDE);
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
        m_IsSilent = checkbox[JSILENCE].m_state;
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
void CGame::ShowMenu()
{
    BITMAP_FILE_PTR bitmap=new BITMAP_FILE;
    bitmap->Load_File(".\\background\\MainMenu.bmp");
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    for (int i = ISINGLE_MODE; i <= IREGISTRY; i++) button[i].Draw();
    return;
}

void CGame::SinglePlayer()
{
    MessageBox(NULL, "Function not defined", "Attention", MB_OK);
    return;
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

void CGame::Settings()
{
    BITMAP_FILE_PTR bitmap = new BITMAP_FILE;
    bitmap->Load_File(".\\background\\Setting.bmp"); //wait for load
    DDraw_Draw_Bitmap(bitmap, lpddsback, { 0,0 });
    bitmap->Unload_File();
    checkbox[JSILENCE].Draw();
    button[IRETURN].Draw();
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
    MessageBox(NULL, "function Prelude() not defined", "Attention", MB_OK);
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
