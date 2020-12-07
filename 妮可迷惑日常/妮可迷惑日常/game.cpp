#include"game.h"
extern CButton button[20];
//EXTERN_INPUT_DATA()
inline void CGame::SetGameState(int GameState)
{
	m_state = GameState;
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
    m_connected = 0;
    m_loggedin = 0;
}
void CGame::GameMain()
{
    GetCurMsg();
    ProcessButtonMsg();
	switch (m_state)
	{
	case MAINMENU:
     ShowMenu();
 break;
	default:
		break;
	}
    Sleep(30);
}
void CGame::GetCurMsg()
{
    return;
}
void CGame::ProcessButtonMsg()
{
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
    bitmap->Load_File("background\\MainMenu.bmp");LPDIRECTDRAWSURFACE7 lpdds;
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
