#include "GUI.h"
EXTERN_BOB_OBJECTS()
CButton button[20];
CCheckBox checkbox[20];
CInputBox inputbox[5];
CButton::~CButton()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_ButtonSur[i])
		{
			m_ButtonSur[i]->Release();
			m_ButtonSur[i] = NULL;
			m_bitmap[i].Unload_File();
		}
	}
}
void CButton::Create(int ID, int width, int height, int x, int y, std::string filename)
{
	m_ID = ID;
	POINT position_in_offscreen;
	position_in_offscreen.x = 0;
	position_in_offscreen.y = 0;
	BITMAP_FILE T_bitmap[3];
	for (int i = 0; i < 3; i++)
		m_ButtonSur[i] = DDraw_Create_Surface(width, height, 0, 0);
	m_Width = width;
	m_Height = height;
	boarder.left = x;
	boarder.top = y;
	boarder.bottom = boarder.top + m_Height;
	boarder.right = boarder.left + m_Width;
	T_bitmap[0].Load_File((".\\GUI\\button\\" + filename + ".bmp").c_str());
	T_bitmap[1].Load_File((".\\GUI\\button\\" + filename + "_on.bmp").c_str());
	T_bitmap[2].Load_File((".\\GUI\\button\\" + filename + "_down.bmp").c_str());
	for (int i = 0; i < 3; i++)
		DDraw_Draw_Bitmap(&T_bitmap[i], m_ButtonSur[i], position_in_offscreen);
	m_state = BSTATENORMAL;
	for (int i = 0; i < 3; ++i)
		T_bitmap[i].Unload_File();
}
void CButton::Draw()//在后备缓冲表面绘图
{
	lpddsback->Blt(&boarder, m_ButtonSur[m_state % 3], NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
}

void CButton::Check()
{
	if (m_clipped == false && m_state != BSTATEUP) m_state = BSTATENORMAL;
	if (pos.x >= boarder.left && pos.x <= boarder.right && pos.y >= boarder.top && pos.y <= boarder.bottom)
	{
		if (m_clipped == false) m_state = BSTATEON;
		else m_state = BSTATEUP;
		if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80)
		{
			m_state = BSTATEDOWN;
			m_clipped = true;
		}
		else m_clipped = false;
	}
	else
	{
		m_clipped = false;
	}
}


CCheckBox::~CCheckBox()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_CheckBoxSur[i])
		{
			m_CheckBoxSur[i]->Release();
			m_CheckBoxSur[i] = NULL;
			m_bitmap[i].Unload_File();
		}
	}
}

void CCheckBox::Create(int ID, int width, int height, int x, int y, std::string filename, bool state)
{
	m_ID = ID;
	POINT position_in_offscreen;
	DDCOLORKEY color_key;
	color_key.dwColorSpaceHighValue = RGBBIT(0,255,255,255);
	color_key.dwColorSpaceLowValue = RGBBIT(0, 255, 255, 255);
	for (int i = 0; i < 2; i++)m_Style[i] = (char*)malloc(30 * sizeof(char));
	for (int i = 0; i < 2; i++)
	{
		m_CheckBoxSur[i] = DDraw_Create_Surface(1000, 100, 0, 0);
		m_CheckBoxSur[i]->SetColorKey(DDCKEY_SRCBLT, &color_key);
	}
	m_Width = width;
	m_Height = height;
	boarder.left = x;
	boarder.top = y;
	boarder.bottom = boarder.top + m_Height;
	boarder.right = boarder.left + m_Width;
	strcpy(m_Style[0], ("GUI\\checkbox\\" + filename + "_on" + ".bmp").c_str());
	strcpy(m_Style[1], ("GUI\\checkbox\\" + filename + "_off" + ".bmp").c_str());
	for (int i = 0; i < 2; i++) m_bitmap[i].Load_File(m_Style[i]);
	for (int i = 0; i < 2; i++)
	{
		position_in_offscreen.x = 0;
		position_in_offscreen.y = 0;
		DDraw_Draw_Bitmap(&m_bitmap[i], m_CheckBoxSur[i], position_in_offscreen);
	}
	m_state = state;
}

void CCheckBox::Draw()//在后备缓冲表面绘图
{
	RECT coor;
	coor.left = 0;
	coor.top = 0;
	coor.right = coor.left + m_Width;
	coor.bottom = coor.top + m_Height;
	lpddsback->Blt(&boarder, m_CheckBoxSur[m_state], &coor, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
}

void CCheckBox::Check()//在后备缓冲表面绘图
{
	if (pos.x >= boarder.left && pos.x <= boarder.right && pos.y >= boarder.top && pos.y <= boarder.bottom)
		if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80) {
			if (GetTickCount() - m_ClipTime >= 500) {
				m_state ^= 1;
				m_ClipTime = GetTickCount();
			}
		}
}
void CInputBox::Create(int ID, int width, int height, int x, int y, bool password)
{
	m_ID = ID;
	m_Width = width;
	m_Height = height;
	m_last_input_time = clock();
	memset(m_input, 0, sizeof(m_input));
	m_input[0] = '|';
	boarder.left = x;
	boarder.top = y;
	boarder.bottom = boarder.top + m_Height;
	boarder.right = boarder.left + m_Width;
}

void CInputBox::Draw()
{
	DDBLTFX ddbltfx;
	static clock_t last_draw_time=clock();
	static bool character = false;//should buf's last character be '|'
	if (clock() - last_draw_time >= 600)
	{
		character = !character;
		last_draw_time = clock();
	}
	DDRAW_INIT_STRUCT(ddbltfx);
	ddbltfx.dwFillColor = RGBBIT(0, 255, 255, 255);
	lpddsback->Blt(&boarder, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
	POINT beginning;
	beginning.x = boarder.left + 5;
	beginning.y = boarder.top + 5;
	if (!m_password)
	{
		char buf[50];
		memcpy(buf, m_input, sizeof(m_input));
		if (!m_activated||!character) buf[strlen(buf) - 1] = 0;
		Draw_Text_GDI(buf, beginning.x, beginning.y, RGB(0, 0, 0), lpddsback);
	}
	else
	{
		char buf[50];
		for (int i = 1; i < strlen(m_input); i++) buf[i] = '*';
		buf[strlen(m_input) - 1] = 0;
		Draw_Text_GDI(buf, beginning.x, beginning.y, RGB(0, 0, 0), lpddsback);
	}
}

void CInputBox::Check()
{
	if (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80)
	{
		if (pos.x >= boarder.left && pos.x <= boarder.right && pos.y >= boarder.top && pos.y <= boarder.bottom)
		{
			m_activated = true;
		}
		else
		{
			m_activated = false;
		}
	}
	if (!m_activated) return;
	if (keyboard_state[DIK_0] & 0x80 && (m_last_input != '0' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input) - 1] = '0'; m_last_input = '0'; }
	else if (keyboard_state[DIK_1] & 0x80 && (m_last_input != '1' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '1'; m_last_input = '1'; }
	else if (keyboard_state[DIK_2] & 0x80 && (m_last_input != '2' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '2'; m_last_input = '2'; }
	else if (keyboard_state[DIK_3] & 0x80 && (m_last_input != '3' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '3'; m_last_input = '3'; }
	else if (keyboard_state[DIK_4] & 0x80 && (m_last_input != '4' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '4'; m_last_input = '4'; }
	else if (keyboard_state[DIK_5] & 0x80 && (m_last_input != '5' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '5'; m_last_input = '5'; }
	else if (keyboard_state[DIK_6] & 0x80 && (m_last_input != '6' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '6'; m_last_input = '6'; }
	else if (keyboard_state[DIK_7] & 0x80 && (m_last_input != '7' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '7'; m_last_input = '7'; }
	else if (keyboard_state[DIK_8] & 0x80 && (m_last_input != '8' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '8'; m_last_input = '8'; }
	else if (keyboard_state[DIK_9] & 0x80 && (m_last_input != '9' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '9'; m_last_input = '9'; }
	else if (keyboard_state[DIK_PERIOD] & 0x80 && (m_last_input != '.' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '.'; m_last_input = '.'; }
	else if (keyboard_state[DIK_NUMPAD0] & 0x80 && (m_last_input != '0' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '0'; m_last_input = '0'; }
	else if (keyboard_state[DIK_NUMPAD1] & 0x80 && (m_last_input != '1' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '1'; m_last_input = '1'; }
	else if (keyboard_state[DIK_NUMPAD2] & 0x80 && (m_last_input != '2' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '2'; m_last_input = '2'; }
	else if (keyboard_state[DIK_NUMPAD3] & 0x80 && (m_last_input != '3' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '3'; m_last_input = '3'; }
	else if (keyboard_state[DIK_NUMPAD4] & 0x80 && (m_last_input != '4' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '4'; m_last_input = '4'; }
	else if (keyboard_state[DIK_NUMPAD5] & 0x80 && (m_last_input != '5' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '5'; m_last_input = '5'; }
	else if (keyboard_state[DIK_NUMPAD6] & 0x80 && (m_last_input != '6' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '6'; m_last_input = '6'; }
	else if (keyboard_state[DIK_NUMPAD7] & 0x80 && (m_last_input != '7' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '7'; m_last_input = '7'; }
	else if (keyboard_state[DIK_NUMPAD8] & 0x80 && (m_last_input != '8' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '8'; m_last_input = '8'; }
	else if (keyboard_state[DIK_NUMPAD9] & 0x80 && (m_last_input != '9' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '9'; m_last_input = '9'; }
	else if (keyboard_state[DIK_NUMPADPERIOD] & 0x80 && (m_last_input != '.' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input)-1] = '.'; m_last_input = '.'; }
	else if ((keyboard_state[DIK_A] & 0x80) && (m_last_input != 'A' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'A'; m_last_input = 'A'; }
	else if ((keyboard_state[DIK_B] & 0x80) && (m_last_input != 'B' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'B'; m_last_input = 'B'; }
	else if ((keyboard_state[DIK_C] & 0x80) && (m_last_input != 'C' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'C'; m_last_input = 'C'; }
	else if ((keyboard_state[DIK_D] & 0x80) && (m_last_input != 'D' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'D'; m_last_input = 'D'; }
	else if ((keyboard_state[DIK_E] & 0x80) && (m_last_input != 'E' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'E'; m_last_input = 'E'; }
	else if ((keyboard_state[DIK_F] & 0x80) && (m_last_input != 'F' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'F'; m_last_input = 'F'; }
	else if ((keyboard_state[DIK_G] & 0x80) && (m_last_input != 'G' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'G'; m_last_input = 'G'; }
	else if ((keyboard_state[DIK_H] & 0x80) && (m_last_input != 'H' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'H'; m_last_input = 'H'; }
	else if ((keyboard_state[DIK_I] & 0x80) && (m_last_input != 'I' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'I'; m_last_input = 'I'; }
	else if ((keyboard_state[DIK_J] & 0x80) && (m_last_input != 'J' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'J'; m_last_input = 'J'; }
	else if ((keyboard_state[DIK_K] & 0x80) && (m_last_input != 'K' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'K'; m_last_input = 'K'; }
	else if ((keyboard_state[DIK_L] & 0x80) && (m_last_input != 'L' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'L'; m_last_input = 'L'; }
	else if ((keyboard_state[DIK_M] & 0x80) && (m_last_input != 'M' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'M'; m_last_input = 'M'; }
	else if ((keyboard_state[DIK_N] & 0x80) && (m_last_input != 'N' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'N'; m_last_input = 'N'; }
	else if ((keyboard_state[DIK_O] & 0x80) && (m_last_input != 'O' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'O'; m_last_input = 'O'; }
	else if ((keyboard_state[DIK_P] & 0x80) && (m_last_input != 'P' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'P'; m_last_input = 'P'; }
	else if ((keyboard_state[DIK_Q] & 0x80) && (m_last_input != 'Q' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'Q'; m_last_input = 'Q'; }
	else if ((keyboard_state[DIK_R] & 0x80) && (m_last_input != 'R' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'R'; m_last_input = 'R'; }
	else if ((keyboard_state[DIK_S] & 0x80) && (m_last_input != 'S' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'S'; m_last_input = 'S'; }
	else if ((keyboard_state[DIK_T] & 0x80) && (m_last_input != 'T' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'T'; m_last_input = 'T'; }
	else if ((keyboard_state[DIK_U] & 0x80) && (m_last_input != 'U' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'U'; m_last_input = 'U'; }
	else if ((keyboard_state[DIK_V] & 0x80) && (m_last_input != 'V' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'V'; m_last_input = 'V'; }
	else if ((keyboard_state[DIK_W] & 0x80) && (m_last_input != 'W' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'W'; m_last_input = 'W'; }
	else if ((keyboard_state[DIK_X] & 0x80) && (m_last_input != 'X' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'X'; m_last_input = 'X'; }
	else if ((keyboard_state[DIK_Y] & 0x80) && (m_last_input != 'Y' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'Y'; m_last_input = 'Y'; }
	else if ((keyboard_state[DIK_Z] & 0x80) && (m_last_input != 'Z' || clock() - m_last_input_time >= 300) && (keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'Z'; m_last_input = 'Z'; }
	else if ((keyboard_state[DIK_A] & 0x80) && (m_last_input != 'a' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'a'; m_last_input = 'a'; }
	else if ((keyboard_state[DIK_B] & 0x80) && (m_last_input != 'b' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'b'; m_last_input = 'b'; }
	else if ((keyboard_state[DIK_C] & 0x80) && (m_last_input != 'c' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'c'; m_last_input = 'c'; }
	else if ((keyboard_state[DIK_D] & 0x80) && (m_last_input != 'd' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'd'; m_last_input = 'd'; }
	else if ((keyboard_state[DIK_E] & 0x80) && (m_last_input != 'e' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'e'; m_last_input = 'e'; }
	else if ((keyboard_state[DIK_F] & 0x80) && (m_last_input != 'f' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'f'; m_last_input = 'f'; }
	else if ((keyboard_state[DIK_G] & 0x80) && (m_last_input != 'g' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'g'; m_last_input = 'g'; }
	else if ((keyboard_state[DIK_H] & 0x80) && (m_last_input != 'h' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'h'; m_last_input = 'h'; }
	else if ((keyboard_state[DIK_I] & 0x80) && (m_last_input != 'i' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'i'; m_last_input = 'i'; }
	else if ((keyboard_state[DIK_J] & 0x80) && (m_last_input != 'j' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'j'; m_last_input = 'j'; }
	else if ((keyboard_state[DIK_K] & 0x80) && (m_last_input != 'k' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'k'; m_last_input = 'k'; }
	else if ((keyboard_state[DIK_L] & 0x80) && (m_last_input != 'l' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'l'; m_last_input = 'l'; }
	else if ((keyboard_state[DIK_M] & 0x80) && (m_last_input != 'm' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'm'; m_last_input = 'm'; }
	else if ((keyboard_state[DIK_N] & 0x80) && (m_last_input != 'n' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'n'; m_last_input = 'n'; }
	else if ((keyboard_state[DIK_O] & 0x80) && (m_last_input != 'o' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'o'; m_last_input = 'o'; }
	else if ((keyboard_state[DIK_P] & 0x80) && (m_last_input != 'p' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'p'; m_last_input = 'p'; }
	else if ((keyboard_state[DIK_Q] & 0x80) && (m_last_input != 'q' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'q'; m_last_input = 'q'; }
	else if ((keyboard_state[DIK_R] & 0x80) && (m_last_input != 'r' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'w'; m_last_input = 'r'; }
	else if ((keyboard_state[DIK_S] & 0x80) && (m_last_input != 's' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 's'; m_last_input = 's'; }
	else if ((keyboard_state[DIK_T] & 0x80) && (m_last_input != 't' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 't'; m_last_input = 't'; }
	else if ((keyboard_state[DIK_U] & 0x80) && (m_last_input != 'u' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'u'; m_last_input = 'u'; }
	else if ((keyboard_state[DIK_V] & 0x80) && (m_last_input != 'v' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'v'; m_last_input = 'v'; }
	else if ((keyboard_state[DIK_W] & 0x80) && (m_last_input != 'w' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'w'; m_last_input = 'w'; }
	else if ((keyboard_state[DIK_X] & 0x80) && (m_last_input != 'x' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'x'; m_last_input = 'x'; }
	else if ((keyboard_state[DIK_Y] & 0x80) && (m_last_input != 'y' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'y'; m_last_input = 'y'; }
	else if ((keyboard_state[DIK_Z] & 0x80) && (m_last_input != 'z' || clock() - m_last_input_time >= 300) && !(keyboard_state[DIK_LSHIFT] & 0x80 || (keyboard_state[DIK_RSHIFT] & 0x80))) { m_input[strlen(m_input)-1] = 'z'; m_last_input = 'z'; }
	else if (keyboard_state[DIK_BACKSPACE] & 0x80 && (m_last_input != '\n' || clock() - m_last_input_time >= 300)) { m_input[strlen(m_input) - 1] = 0; m_input[strlen(m_input) - 1] = 0; m_last_input = '\n'; }
	else return;
	m_input[strlen(m_input)] = '|';
	m_last_input_time = clock();
	return;
}
