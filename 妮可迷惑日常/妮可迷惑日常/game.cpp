#include"game.h"

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
    m_connected = 0;
    m_loggedin = 0;
}
void CGame::GameMain()
{
	switch (m_state)
	{
	case MAINMENU:
     ShowMenu(); break;
	default:
		break;
	}
}
void CGame::ShowMenu()
{
    DDSURFACEDESC2 ddsd;  //  direct draw surface description 

    UINT* source_ptr,   // working pointers
        * dest_ptr;
    BITMAP_FILE bi;
    BITMAP_FILE_PTR bitmap=&bi;
    bitmap->Load_File("background\\MainMenu.bmp");
    // extract bitmap data
    source_ptr = bitmap->buffer;
    // get the addr to destination surface memory

    // set size of the structure
    ddsd.dwSize = sizeof(ddsd);
    LPDIRECTDRAWSURFACE7 lpdds;
    lpdds=DDraw_Create_Surface(800, 600, 0, 0);
    RECT rect_list = { 0,0,800,600 };
    RECT rect_list2,rect_list3;
    GetWindowRect(main_window_handle,&rect_list2);
    GetClientRect(main_window_handle, &rect_list3);
    rect_list2.top += rect_list2.bottom - rect_list2.top - rect_list3.bottom-8;
    // lock the display surface
    if (FAILED((lpdds)->Lock(NULL,
        &ddsd,
        DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,
        NULL))) return;

    // assign a pointer to the memory surface for manipulation
    dest_ptr = (UINT*)ddsd.lpSurface;

    // iterate thru each scanline and copy bitmap
    int lpitch = (int)(ddsd.lPitch >> 2);
    for (int index_y = 0; index_y < bitmap->bitmapinfoheader.biHeight; index_y++)
    {
        memcpy(dest_ptr, source_ptr, bitmap->bitmapinfoheader.biWidth * sizeof(UINT)); // copy next line of data to destination

        // advance pointers
        dest_ptr += lpitch;
        source_ptr += bitmap->bitmapinfoheader.biWidth;
    } // end for index_y

    (lpdds)->Unlock(NULL); // unlock the surface 
    lpddsprimary->Blt(&rect_list2, lpdds, &rect_list,DDBLT_WAIT, NULL);
    lpdds->Release();
    bitmap->Unload_File();
    return;
}
