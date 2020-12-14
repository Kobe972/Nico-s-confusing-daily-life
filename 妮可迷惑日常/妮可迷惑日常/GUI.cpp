#include "GUI.h"
EXTERN_BOB_OBJECTS()
CButton button[20];
CCheckBox checkbox[20];
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
void CButton::init_by_ID(int ID)
{
	m_ID = ID;
	POINT position_in_offscreen;
	for(int i=0;i<3;i++)m_Style[i]=(char*)malloc(30 * sizeof(char));
	for (int i = 0; i < 3; i++)
	{ 
		m_ButtonSur[i] = DDraw_Create_Surface(1000, 100,0,0);
	}
	switch (m_ID)
	{
	case ISINGLE_MODE:
		m_Width = 271;
		m_Height = 63;
		boarder.left = 250;
		boarder.top = 160;
		boarder.bottom = boarder.top + m_Height;
		boarder.right = boarder.left + m_Width;
		strcpy(m_Style[0], "GUI\\button\\button1.bmp");
		strcpy(m_Style[1], "GUI\\button\\button1_on.bmp");
		strcpy(m_Style[2], "GUI\\button\\button1_down.bmp");
		for (int i = 0; i < 3; i++) m_bitmap[i].Load_File(m_Style[i]);
		for (int i = 0; i < 3; i++)
		{
			position_in_offscreen.x = m_Width * i;
			position_in_offscreen.y = 0;
			DDraw_Draw_Bitmap(&m_bitmap[i], m_ButtonSur[i], position_in_offscreen);
		}
		m_state = BSTATENORMAL;
		break;
	case IMULTIMODE:
		m_Width = 271;
		m_Height = 63;
		boarder.left = 250;
		boarder.top = 243;
		boarder.bottom = boarder.top + m_Height;
		boarder.right = boarder.left + m_Width;
		strcpy(m_Style[0], "GUI\\button\\button2.bmp");
		strcpy(m_Style[1], "GUI\\button\\button2_on.bmp");
		strcpy(m_Style[2], "GUI\\button\\button2_down.bmp");
		for (int i = 0; i < 3; i++) m_bitmap[i].Load_File(m_Style[i]);
		for (int i = 0; i < 3; i++)
		{
			position_in_offscreen.x = m_Width * i;
			position_in_offscreen.y = 0;
			DDraw_Draw_Bitmap(&m_bitmap[i], m_ButtonSur[i], position_in_offscreen);
		}
		m_state = BSTATENORMAL;
		break;
	case ISETTINGS:
		m_Width = 271;
		m_Height = 63;
		boarder.left = 250;
		boarder.top = 326;
		boarder.bottom = boarder.top + m_Height;
		boarder.right = boarder.left + m_Width;
		strcpy(m_Style[0], "GUI\\button\\button3.bmp");
		strcpy(m_Style[1], "GUI\\button\\button3_on.bmp");
		strcpy(m_Style[2], "GUI\\button\\button3_down.bmp");
		for (int i = 0; i < 3; i++) m_bitmap[i].Load_File(m_Style[i]);
		for (int i = 0; i < 3; i++)
		{
			position_in_offscreen.x = m_Width * i;
			position_in_offscreen.y = 0;
			DDraw_Draw_Bitmap(&m_bitmap[i], m_ButtonSur[i], position_in_offscreen);
		}
		m_state = BSTATENORMAL;
		break;
	case IHELP:
		m_Width = 271;
		m_Height = 63;
		boarder.left = 250;
		boarder.top = 409;
		boarder.bottom = boarder.top + m_Height;
		boarder.right = boarder.left + m_Width;
		strcpy(m_Style[0], "GUI\\button\\button4.bmp");
		strcpy(m_Style[1], "GUI\\button\\button4_on.bmp");
		strcpy(m_Style[2], "GUI\\button\\button4_down.bmp");
		for (int i = 0; i < 3; i++) m_bitmap[i].Load_File(m_Style[i]);
		for (int i = 0; i < 3; i++)
		{
			position_in_offscreen.x = m_Width * i;
			position_in_offscreen.y = 0;
			DDraw_Draw_Bitmap(&m_bitmap[i], m_ButtonSur[i], position_in_offscreen);
		}
		m_state = BSTATENORMAL;
		break;
	case ILOG:
		m_Width = 150;
		m_Height = 62;
		boarder.left = 50;
		boarder.top = 500;
		boarder.bottom = boarder.top + m_Height;
		boarder.right = boarder.left + m_Width;
		strcpy(m_Style[0], "GUI\\button\\log.bmp");
		strcpy(m_Style[1], "GUI\\button\\log_on.bmp");
		strcpy(m_Style[2], "GUI\\button\\log_down.bmp");
		for (int i = 0; i < 3; i++) m_bitmap[i].Load_File(m_Style[i]);
		for (int i = 0; i < 3; i++)
		{
			position_in_offscreen.x = m_Width * i;
			position_in_offscreen.y = 0;
			DDraw_Draw_Bitmap(&m_bitmap[i], m_ButtonSur[i], position_in_offscreen);
		}
		m_state = BSTATENORMAL;
		break;
	case IREGISTRY:
		m_Width = 150;
		m_Height = 62;
		boarder.left = 220;
		boarder.top = 500;
		boarder.bottom = boarder.top + m_Height;
		boarder.right = boarder.left + m_Width;
		strcpy(m_Style[0], "GUI\\button\\registry.bmp");
		strcpy(m_Style[1], "GUI\\button\\registry_on.bmp");
		strcpy(m_Style[2], "GUI\\button\\registry_down.bmp");
		for (int i = 0; i < 3; i++) m_bitmap[i].Load_File(m_Style[i]);
		for (int i = 0; i < 3; i++)
		{
			position_in_offscreen.x = m_Width * i;
			position_in_offscreen.y = 0;
			DDraw_Draw_Bitmap(&m_bitmap[i], m_ButtonSur[i], position_in_offscreen);
		}
		m_state = BSTATENORMAL;
		break;
	case IRETURN:
		m_Width = 80;
		m_Height = 80;
		boarder.left = 0;
		boarder.top = 0;
		boarder.bottom = boarder.top + m_Height;
		boarder.right = boarder.left + m_Width;
		strcpy(m_Style[0], "GUI\\button\\return.bmp");
		strcpy(m_Style[1], "GUI\\button\\return_on.bmp");
		strcpy(m_Style[2], "GUI\\button\\return_down.bmp");
		for (int i = 0; i < 3; i++) m_bitmap[i].Load_File(m_Style[i]);
		for (int i = 0; i < 3; i++)
		{
			position_in_offscreen.x = m_Width * i;
			position_in_offscreen.y = 0;
			DDraw_Draw_Bitmap(&m_bitmap[i], m_ButtonSur[i], position_in_offscreen);
		}
		m_state = BSTATENORMAL;;
		break;
	default:
		break;
	}
}
void CButton::Draw()//在后备缓冲表面绘图
{
	RECT coor;
	coor.left = (m_state % 3) * m_Width;
	coor.top = 0;
	coor.right = coor.left + m_Width;
	coor.bottom = coor.top + m_Height;
	lpddsback->Blt(&boarder, m_ButtonSur[m_state % 3], &coor, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
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

void CCheckBox::init_by_ID(int ID, int width, int height, int x, int y, std::string filename, bool state)
{
	m_ID = ID;
	POINT position_in_offscreen;
	for (int i = 0; i < 2; i++)m_Style[i] = (char*)malloc(30 * sizeof(char));
	for (int i = 0; i < 2; i++)
	{
		m_CheckBoxSur[i] = DDraw_Create_Surface(1000, 100, 0, 0);
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