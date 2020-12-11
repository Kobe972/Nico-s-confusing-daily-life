#include"game.h"
extern CButton button[20];
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
    for(int i=ISINGLE_MODE;i<=IREGISTRY;i++)
        button[i].init_by_ID(i);//The ID of the ith button is i
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
    break;
	default:
		break;
	}
    GetCurMsg();
    ProcessButtonMsg();
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
            }
        }
    }
    return;
}
void CGame::ProcessKeyMsg()
{
    return;
}
void CGame::ShowMenu()
{
    UINT* source_ptr,   // working pointers
        * dest_ptr;
    BITMAP_FILE_PTR bitmap=new BITMAP_FILE;
    bitmap->Load_File(".\\background\\MainMenu.bmp");LPDIRECTDRAWSURFACE7 lpdds;
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
    MessageBox(NULL, "Function not defined", "Attention", MB_OK);
    return;
}

void CGame::Settings()
{
    MessageBox(NULL, "Function not defined", "Attention", MB_OK);
    return;
}

void CGame::SelectSkin()
{
    MessageBox(NULL, "Function not defined", "Attention", MB_OK);
    return;
}

void CGame::SelectHardship()
{
    MessageBox(NULL, "Function not defined", "Attention", MB_OK);
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
CGame::~CGame()
{
    DInput_Release_Keyboard();
    DInput_Read_Mouse();
    DInput_Shutdown();
    DDraw_Shutdown();
}
