#include "map.h"
#include "BobClass.h"
EXTERN_BOB_OBJECTS()
EXTERN_INPUT_DATA()
CStaticObstacle staticobstacle[20];

void CStaticObstacle::Create(bool safetop, bool safebottom, bool safeleft, bool saferight, std::string filename)
{
	m_safe.top = safetop;
	m_safe.bottom = safebottom;
	m_safe.left = safeleft;
	m_safe.right = saferight;
	BITMAP_FILE T_bitmap;
	T_bitmap.Load_File((".\\Map\\StaticObstacle\\" + filename + ".bmp").c_str());
	m_Surface = DDraw_Create_Surface(T_bitmap.bitmapinfoheader.biWidth, T_bitmap.bitmapinfoheader.biHeight);
	POINT T_coor;
	T_coor.x = T_coor.y = 0;
	DDraw_Draw_Bitmap(&T_bitmap, m_Surface, T_coor);
	T_bitmap.Unload_File();
}
void CStaticObstacle::Draw(RECT border)
{
	lpddsback->Blt(&border, m_Surface, NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
}

inline int Rand(int L, int R) { return rand() % (R - L + 1) + L; }

void CMap::Load_Silence(int left, int length)
{
	int T_CurLength;
	RECT T_border;
	for (; length >= SILENCEGAPHIGH;) {
		T_CurLength = Rand(SILENCEGAPLOW, SILENCEGAPHIGH);
		T_border.bottom = SCREEN_HEIGHT;
		T_border.top = SCREEN_HEIGHT - Rand(SILENCEPLANELOW, SILENCEPLANEHIGH);
		T_border.left = left;
		T_border.right = left + T_CurLength;
		length -= T_CurLength;
		m_field[m_fieldCounter++] = T_border;
		left += T_CurLength;
	}
	T_border.bottom = SCREEN_HEIGHT;
	T_border.top = SCREEN_HEIGHT - Rand(SILENCEPLANELOW, SILENCEPLANEHIGH);
	T_border.left = left;
	T_border.right = left + length;
	m_field[m_fieldCounter++] = T_border;
}

void CMap::Create(int ModelNumber, int Hardness, int SilenceLow, int SilenceHigh) 
{
	m_Length = 0;
	m_Left = 0;
	m_Right = SCREEN_WIDTH;
	m_ObstacleCounter = 0;
	m_fieldCounter = 0 ;
	m_ObstacleCur = 0;
	m_fieldCur = 0;
	for (int i = 0; i < ModelNumber; ++i) 
	{
		int T_CurSilenceLength = Rand(SilenceLow, SilenceHigh);
		Load_Silence(m_Length, T_CurSilenceLength);
		m_Length += T_CurSilenceLength;
		m_Length += (this->*m_pfnLoad_Model[2][0])(m_Length);
	}
}

RECT TOABS(RECT Cur, int left) {
	Cur.left -= left;
	Cur.right -= left;
	return Cur;
}

void CMap::Draw() 
{
	m_BarrierNumber = 0;
	RECT T_CurBarrier;
	for (int i = m_fieldCur; i < m_fieldCounter; ++i) 
	{
		if (m_field[i].left >= m_Right)
			break;
		T_CurBarrier = TOABS(m_field[i], m_Left);
		staticobstacle[SOFIELD].Draw(T_CurBarrier);

		m_CurBarrier[m_BarrierNumber] = T_CurBarrier;
		m_CurSafe[m_BarrierNumber] = staticobstacle[SOFIELD].m_safe;
		m_BarrierNumber++;
	}

	for (int i = m_ObstacleCur; i < m_ObstacleCounter; ++i)
	{
		if (m_Obstacle[i].left >= m_Right)
			break;
		T_CurBarrier = TOABS(m_Obstacle[i], m_Left);
		staticobstacle[m_ObstacleKind[i]].Draw(T_CurBarrier);

		m_CurBarrier[m_BarrierNumber] = T_CurBarrier;
		m_CurSafe[m_BarrierNumber] = staticobstacle[m_ObstacleKind[i]].m_safe;
		m_BarrierNumber++;
	}
}

bool CMap::MoveNext()
{
	m_Left += MAPSPEED;
	m_Right += MAPSPEED;
	for (; m_fieldCur < m_fieldCounter && m_field[m_fieldCur].right < m_Left;)
		m_fieldCur++;
	if (m_fieldCur == m_fieldCounter) return false;
	for (; m_ObstacleCur < m_ObstacleCounter && m_Obstacle[m_ObstacleCur].right < m_Left;)
		m_ObstacleCur++;
	return true;
}

RECT GetRect(int left, int right, int top, int bottom) {
	RECT T_rect;
	T_rect.left = left;
	T_rect.right = right;
	T_rect.top = top;
	T_rect.bottom = bottom;
	return T_rect;
}

void CMap::AddField(int left, int right, int height) 
{
	m_field[m_fieldCounter++] = GetRect(left, right, height, 800);
}

void CMap::AddStObstacle(int left, int right, int low, int high, int kind) 
{
	m_Obstacle[m_ObstacleCounter] = GetRect(left, right, low, high);
	m_ObstacleKind[m_ObstacleCounter] = kind;
	m_ObstacleCounter++;
}


int CMap::BaseHardV(int left)
{
	AddField(left, left + 400, 400);
	left += 320;
	AddStObstacle(left, left + 80, 0, 320, SOPILLAR);
	left += 780;
	AddField(left, left + 400, 400);
	AddStObstacle(left, left + 80, 0, 240, SOPILLAR);
	return 1500;
}

int CMap::BaseHardUUDD(int left)
{
	AddField(left, left + 2000, 500);
	left += 350;
	AddStObstacle(left, left + 100, 400, 500, SOMATHBOOK);
	left += 350;
	AddStObstacle(left, left + 80, 0, 420, SOPILLAR);
	left += 330;
	AddStObstacle(left, left + 100, 400, 500, SOMATHBOOK);
	left += 350;
	AddStObstacle(left, left + 80, 0, 420, SOPILLAR);
	left += 330;
	AddStObstacle(left, left + 100, 400, 500, SOMATHBOOK);
	return 2000;
}

int CMap::BaseHardW(int left)
{
	AddField(left, left + 400, 400);
	left += 650;
	AddStObstacle(left, left + 80, 0, 325, SOPILLAR);
	left += 330;
	AddField(left, left + 400, 400);
	left += 650;
	AddStObstacle(left, left + 80, 0, 325, SOPILLAR);
	left += 330;
	AddField(left, left + 400, 400);
	return 2200;
}

int CMap::BaseHardJump(int left)
{
	int T_preleft = left;
	AddField(left, left + 500, 500);
	left += 500;
	AddField(left, left + 400, 300);
	left += 320;
	AddStObstacle(left, left + 80, 0, 220, SOPILLAR);
	left += 80;
	AddField(left, left + 1000, 500);
	left += 400;
	AddStObstacle(left, left + 100, 400, 500, SOMATHBOOK);
	left += 520;
	AddStObstacle(left, left + 80, 0, 420, SOPILLAR);
	left += 80;
	AddField(left, left + 400, 500);
	left += 400;
	return left - T_preleft;
}

int CMap::Load_Model_Hardness2_Id0(int left) 
{ 
	int T_preleft = left;
	left += BaseHardV(left);
	left += BaseHardW(left);
	left += BaseHardUUDD(left);
	left += BaseHardJump(left);
	return left - T_preleft;
}

int CMap::Load_Model_Hardness2_Id1(int left) 
{
	return 0;
}


int CMap::Load_Model_Hardness2_Id2(int left)
{
	return 0;
}


int CMap::Load_Model_Hardness2_Id3(int left)
{
	return 0;
}