#include "GUI.h"
EXTERN_BOB_OBJECTS()
CButton button[20];
CButton::~CButton()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_ButtonSur[i])
		{
			m_ButtonSur[i]->Release();
			m_ButtonSur[i] = NULL;
		}
	}
}
void CButton::init_by_ID(int ID)
{
	m_ID = ID;
	POINT position_in_offscreen;
	for(int i=0;i<3;i++)m_Style[i]=(char*)malloc(30 * sizeof(char));
	for(int i=0;i<3;i++)m_ButtonSur[i] = DDraw_Create_Surface(1000, 100);
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
	default:
		break;
	}
}
void CButton::Draw()
{
	RECT coor;
	coor.left = (m_state%3)*m_Width; 
	coor.top = 0;
	coor.right = coor.left + m_Width;
	coor.bottom = coor.top + m_Height;
	lpddsback->Blt(&boarder, m_ButtonSur[m_state % 3], &coor, DDBLT_WAIT, NULL);
}

