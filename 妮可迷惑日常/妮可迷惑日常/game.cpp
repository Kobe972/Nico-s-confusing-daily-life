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
    SetGameState(MAINMENU);
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
            if (button[i].m_clipped == false&&button[i].m_state!=BSTATEUP) button[i].m_state = BSTATENORMAL;
            if (pos.x >= button[i].boarder.left && pos.x <= button[i].boarder.right && pos.y >= button[i].boarder.top && pos.y <= button[i].boarder.bottom)
            {
                if (button[i].m_clipped == false) button[i].m_state = BSTATEON;
                else button[i].m_state = BSTATEUP; 
                if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80)
                {
                    button[i].m_state = BSTATEDOWN;
                    button[i].m_clipped = true;
                }
                else button[i].m_clipped = false;
            }
            else
            {
                button[i].m_clipped = false;
            }
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
