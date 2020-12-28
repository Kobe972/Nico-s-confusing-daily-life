#include "map.h"
#include "BobClass.h"
#include"SaveTools.h"
EXTERN_BOB_OBJECTS()
EXTERN_INPUT_DATA()
CStaticObstacle staticobstacle[20];
CMovingObstacle movingobstacle[20];
CHP hp;
extern Save g_local;
const int g_cnt[3] = { 10, 6, 9 };
int MAPSPEED = 10;
void CStaticObstacle::Create(int safetop, int safebottom, int safeleft, int saferight, int reduce,
	std::string filename)
{
	m_safe.top = safetop;
	m_safe.bottom = safebottom;
	m_safe.left = safeleft;
	m_safe.right = saferight;
	m_HPReduce = reduce;
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

void CMovingObstacle::Create(int safetop, int safebottom, int safeleft, int saferight,
	int reduce, int speed, std::string filename)
{
	m_safe.top = safetop;
	m_safe.bottom = safebottom;
	m_safe.left = safeleft;
	m_safe.right = saferight;
	m_HPReduce = reduce;
	m_Speed = speed;
	BITMAP_FILE T_bitmap;
	T_bitmap.Load_File((".\\Map\\MovingObstacle\\" + filename + ".bmp").c_str());
	m_Surface = DDraw_Create_Surface(T_bitmap.bitmapinfoheader.biWidth, T_bitmap.bitmapinfoheader.biHeight);
	POINT T_coor;
	T_coor.x = T_coor.y = 0;
	DDraw_Draw_Bitmap(&T_bitmap, m_Surface, T_coor);
	T_bitmap.Unload_File();
}


void CMovingObstacle::Draw(RECT border)
{
	lpddsback->Blt(&border, m_Surface, NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
}

void CHP::Load_Frame(std::string filename)
{
	for (int i = 0; i <= 10; ++i) 
	{
		BITMAP_FILE T_bitmap;
		T_bitmap.Load_File((".\\Map\\HP\\" + filename + std::to_string(i) + ".bmp").c_str());
		m_Surface[i] = DDraw_Create_Surface(T_bitmap.bitmapinfoheader.biWidth, T_bitmap.bitmapinfoheader.biHeight);
		POINT T_coor;
		T_coor.x = T_coor.y = 0;
		DDraw_Draw_Bitmap(&T_bitmap, m_Surface[i], T_coor);
		T_bitmap.Unload_File();
	}
}

void CHP::Draw(RECT border, int hp)
{
	lpddsback->Blt(&border, m_Surface[hp], NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

}

void CMap::Load_Silence(int left, int length)
{
	int T_CurLength;
	RECT T_border;
	for (; length >= SILENCEGAPHIGH;) {
		T_CurLength = Rand(SILENCEGAPLOW, SILENCEGAPHIGH);
		T_border.bottom = SCREEN_HEIGHT;
		T_border.top = SCREEN_HEIGHT - 100 * Rand(1, 3);
		T_border.left = left;
		T_border.right = left + T_CurLength;
		length -= T_CurLength;
		m_field[m_fieldCounter++] = T_border;
		left += T_CurLength;
	}
	T_border.bottom = SCREEN_HEIGHT;
	T_border.top = SCREEN_HEIGHT - 200;
	T_border.left = left;
	T_border.right = left + length;
	m_field[m_fieldCounter++] = T_border;
}

RECT GetRect(int left, int right, int top, int bottom) {
	RECT T_rect;
	T_rect.left = left;
	T_rect.right = right;
	T_rect.top = top;
	T_rect.bottom = bottom;
	return T_rect;
}

int Get_Rand_Hard(int hardness)
{
	int num;
	num = rand() % 100;
	if (hardness <= 50)
		if (num < hardness) return 1;
		else return 0;
	else
		if (num < hardness) return 2;
		else if (num < 3 * (100 + hardness) / 4) return 1;
	else return 0;
}

void CMap::Create(int ModelNumber, int Hardness, int SilenceLow, int SilenceHigh) 
{
	m_Length = 0;
	m_Left = 0;
	m_Right = SCREEN_WIDTH;
	m_ObstacleCounter = 0;
	m_fieldCounter = 0 ;
	m_MovingCounter = 0;
	m_ObstacleCur = 0;
	m_fieldCur = 0;
	m_MovingCur = 0;
	AddField(0, 2000, 400);
	m_Length = 2000;
	
	for (int i = 0; i < ModelNumber; ++i) 
	{
		int T_CurSilenceLength = Rand(SilenceLow, SilenceHigh);
		Load_Silence(m_Length, T_CurSilenceLength);
		m_Length += T_CurSilenceLength;
		int hard = Get_Rand_Hard(Hardness);
		int Tcur = Rand(0, g_cnt[hard] - 1);
		if (hard <= 1) MAPSPEED = 10;
		else MAPSPEED = 12;
		if (Hardness <= 15) m_Length += Model_For_Fools(m_Length);
		else m_Length += (this->*m_pfnLoad_Model[hard][Tcur])(m_Length);
	}
}

RECT TOABS(RECT Cur, int left) {
	Cur.left -= left;
	Cur.right -= left;
	return Cur;
}

void CMap::Draw(int playerhp) 
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
		if(m_ObstacleKind[i] != -1)
		{
			if (m_Obstacle[i].left >= m_Right)
				break;
			T_CurBarrier = TOABS(m_Obstacle[i], m_Left);
			staticobstacle[m_ObstacleKind[i]].Draw(T_CurBarrier);

			m_CurBarrier[m_BarrierNumber] = T_CurBarrier;
			m_CurSafe[m_BarrierNumber] = staticobstacle[m_ObstacleKind[i]].m_safe;
			m_BarrierNumber++;
		}

	for (int i = m_MovingCur; i < m_MovingCounter; ++i)
		if (m_MovingKind[i] != -1)
		{
			if (m_Moving[i].left >= m_Right)
				break;
			T_CurBarrier = TOABS(m_Moving[i], m_Left);
			movingobstacle[m_MovingKind[i]].Draw(T_CurBarrier);

			m_CurBarrier[m_BarrierNumber] = T_CurBarrier;
			m_CurSafe[m_BarrierNumber] = movingobstacle[m_MovingKind[i]].m_safe;
			m_BarrierNumber++;
		}
	if(playerhp <= 10)
		hp.Draw(GetRect(400, 800, 0, 100), playerhp);
}

int CMap::Fix()
{
	int T_BarrierId = 0, T_HPReduce = 0;
	for (int i = m_fieldCur; i < m_fieldCounter; ++i)
	{
		if (m_field[i].left >= m_Right)
			break;
		T_BarrierId++;
	}

	for (int i = m_ObstacleCur; i < m_ObstacleCounter; ++i)
		if (m_ObstacleKind[i] != -1)
		{
			if (m_Obstacle[i].left >= m_Right)
				break;
			if (m_CurSafe[T_BarrierId].top == 200) {
				T_HPReduce += staticobstacle[m_ObstacleKind[i]].m_HPReduce;
				g_local.iStObstacleFailure[m_ObstacleKind[i]]++;
				m_ObstacleKind[i] = -1;
			}
			T_BarrierId++;
		}

	for (int i = m_MovingCur; i < m_MovingCounter; ++i)
		if (m_MovingKind[i] != -1)
		{
			if (m_Moving[i].left >= m_Right)
				break;
			if (m_CurSafe[T_BarrierId].top == 200) {
				T_HPReduce += movingobstacle[m_MovingKind[i]].m_HPReduce;
				g_local.iMoObstacleFailure[m_MovingKind[i]]++;
				m_MovingKind[i] = -1;
			}
			m_Moving[i].left -= movingobstacle[m_MovingKind[i]].m_Speed;
			m_Moving[i].right -= movingobstacle[m_MovingKind[i]].m_Speed;
			T_BarrierId++;
		}
	return T_HPReduce;
}

bool CMap::MoveNext()
{
	m_Left += MAPSPEED;
	m_Right += MAPSPEED;
	for (; m_fieldCur < m_fieldCounter && m_field[m_fieldCur].right < m_Left;)
		m_fieldCur++;
	if (m_fieldCur == m_fieldCounter) return false;
	for (; m_ObstacleCur < m_ObstacleCounter && m_Obstacle[m_ObstacleCur].right < m_Left;) {
		g_local.iStObstacleSuccess[m_ObstacleKind[m_ObstacleCur]]++;
		m_ObstacleCur++;
	}
	for (; m_MovingCur < m_MovingCounter && m_Moving[m_MovingCur].right < m_Left;) {
		g_local.iMoObstacleSuccess[m_MovingKind[m_MovingCur]]++;
		m_MovingCur++;
	}
	return true;
}

void CMap::AddField(int left, int right, int height) 
{
	m_field[m_fieldCounter++] = GetRect(left, right, height, 600);
}

void CMap::AddStObstacle(int left, int right, int low, int high, int kind) 
{
	m_Obstacle[m_ObstacleCounter] = GetRect(left, right, low, high);
	m_ObstacleKind[m_ObstacleCounter] = kind;
	m_ObstacleCounter++;
}

void CMap::AddMoObstacle(int left, int right, int low, int high, int kind)
{
	m_Moving[m_MovingCounter] = GetRect(left, right, low, high);
	m_MovingKind[m_MovingCounter] = kind;
	m_MovingCounter++;
}


int CMap::BaseHardV(int left)
{
	int T_preleft = left;
	AddField(left, left + 400, 400);
	left += 320;
	AddStObstacle(left, left + 80, 0, 320, SOPILLAR);
	left += 80;
	left += 500;
	AddField(left, left + 400, 400);
	AddStObstacle(left, left + 80, 0, 240, SOPILLAR);
	left += 400;
	return left - T_preleft;
}

int CMap::BaseHardUUDD(int left)
{
	AddField(left, left + 2000, 500);
	left += 350;
	AddStObstacle(left, left + 100, 400, 500, SORANDUPDANGER);
	left += 350;
	AddStObstacle(left, left + 80, 0, 420, SOPILLAR);
	left += 330;
	AddStObstacle(left, left + 100, 400, 500, SORANDUPDANGER);
	left += 350;
	AddStObstacle(left, left + 80, 0, 420, SOPILLAR);
	left += 330;
	AddStObstacle(left, left + 100, 400, 500, SORANDUPDANGER);
	return 2000;
}

int CMap::BaseHardW(int left)
{
	int T_preleft = left;
	AddField(left, left + 400, 400);
	left += 400;
	left += 200;
	AddStObstacle(left, left + 80, 0, 275, SOPILLAR);
	left += 80;
	left += 200;
	AddField(left, left + 400, 400);
	left += 650;
	AddStObstacle(left, left + 80, 0, 275, SOPILLAR);
	left += 330;
	AddField(left, left + 400, 400);
	left += 400;
	return left - T_preleft;
}

int CMap::BaseHardJump(int left)
{
	int T_preleft = left;
	AddField(left, left + 1000, 500);
	left += 500;
	AddStObstacle(left, left + 80, 0, 420, SOPILLAR);
	left += 500;
	AddField(left, left + 400, 300);
	left += 320;
	AddStObstacle(left, left + 80, 0, 220, SOPILLAR);
	left += 80;
	AddField(left, left + 1000, 500);
	left += 300;
	AddStObstacle(left, left + 100, 400, 500, SORANDUPDANGER);
	AddStObstacle(left, left + 100, 0, 100, SORANDUPDANGER);
	left += 420;
	AddStObstacle(left, left + 80, 0, 420, SOPILLAR);
	left += 80;
	AddField(left, left + 400, 500);
	left += 400;
	return left - T_preleft;
}

int CMap::BaseHardCLIMBE(int left)
{
	int T_preleft = left;
	AddField(left, left + 600, 500);
	left += 200;
	AddStObstacle(left, left + 100, 300, 400, SORANDUPDANGER);
	left += 400;
	AddField(left, left + 400, 350);
	AddStObstacle(left, left + 80, 150, 250, SORANDUPDANGER);
	left += 400;
	AddField(left, left + 300, 200);
	left += 300;
	left += 600;
	AddField(left, left + 500, 450);
	AddStObstacle(left, left + 80, 0, 300, SOPILLAR);
	left += 500;
	return left - T_preleft;
}

int CMap::BaseHardCHOICE(int left)
{
	int T_preleft = left;
	AddField(left, left + 400, 300);
	left += 400;
	AddField(left, left + 400, 200);
	left += 320;
	AddStObstacle(left, left + 80, 0, 120, SOPILLAR);
	left += 80;
	AddField(left, left + 400, 300);
	left += 100;
	AddStObstacle(left, left + 300, 0, 200, SORANDUPDANGER);
	left += 100;
	AddField(left, left + 600, 400);
	left += 520;
	AddStObstacle(left, left + 80, 280, 330, SOPILLAR);
	left += 80;
	AddStObstacle(left, left + 400, 500, 600, SORANDUPDANGER);
	left += 200;
	AddStObstacle(left, left + 100, 400, 500, SOJUAN);
	left += 200;
	AddField(left, left + 400, 500);
	left += 400;
	return left - T_preleft;
}


int CMap::BaseHardBIU(int left)
{
	int T_preleft = left;
	AddField(left, left + 800, 400);
	left += 400;
	AddStObstacle(left, left + 80, 0, 320, SOPILLAR);
	left += 400;
	left += 200;
	AddStObstacle(left, left + 100, 120, 200, SOJUAN);
	left += 200;
	AddField(left, left + 500, 400);
	left += 120;
	AddMoObstacle(left, left + 80, 120, 200, MOCONFUSE);
	left += 380;
	return left - T_preleft;
}

int CMap::BaseHardNICONICONI(int left)
{
	int T_preleft = left;
	AddField(left, left + 2000, 400);
	left += 400;
	AddMoObstacle(left, left + 100, 300, 400, MONICO);
	left += 400;
	AddMoObstacle(left, left + 100, 200, 300, MONICO);
	left += 400;
	AddMoObstacle(left, left + 100, 300, 400, MONICO);
	left += 400;
	AddMoObstacle(left, left + 100, 200, 300, MONICO);
	left += 200;
	return left - T_preleft;
}

int CMap::BaseHardNICONINICO(int left)
{

	int T_preleft = left;
	left += 400;
	AddMoObstacle(left, left + 50, 350, 400, MONICO);
	left += 100;
	AddMoObstacle(left, left + 50, 350, 400, MONICO);
	left += 100;
	AddMoObstacle(left, left + 50, 350, 400, MONICO);
	left += 100;
	left += 200;
	AddMoObstacle(left, left + 50, 270, 320, MONICO);
	left += 100;
	AddMoObstacle(left, left + 50, 270, 320, MONICO);
	left += 100;
	AddMoObstacle(left, left + 50, 270, 320, MONICO);
	left += 100;
	AddField(T_preleft, left, 400);
	return left - T_preleft;
}

int CMap::BaseHardNICOCONINI(int left)
{

	int T_preleft = left;
	left += 400;
	AddMoObstacle(left, left + 50, 350, 400, MONICO);
	AddMoObstacle(left, left + 50, 50, 100, MONICO);
	left += 100;
	AddMoObstacle(left, left + 50, 350, 400, MONICO);
	AddMoObstacle(left, left + 50, 50, 100, MONICO);
	left += 100;
	AddMoObstacle(left, left + 50, 350, 400, MONICO);
	AddMoObstacle(left, left + 50, 50, 100, MONICO);
	left += 200;
	AddField(T_preleft, left, 400);
	return left - T_preleft;
}

int CMap::Load_Model_Hardness2_Id0(int left) 
{ 
	int T_preleft = left;
	left += BaseHardV(left);
	left += BaseHardW(left);
	left += BaseHardCLIMBE(left);
	return left - T_preleft;
}

int CMap::Load_Model_Hardness2_Id1(int left) 
{
	int T_preleft = left;
	left += BaseHardJump(left);
	left += BaseHardUUDD(left);
	left += BaseHardBIU(left);
	left += BaseHardCHOICE(left);
	return left - T_preleft;
}


int CMap::Load_Model_Hardness2_Id2(int left)
{
	int T_preleft = left;
	left += BaseHardW(left);
	left += BaseHardUUDD(left);
	left += BaseHardV(left);
	return left - T_preleft;
}


int CMap::Load_Model_Hardness2_Id3(int left)
{
	int T_preleft = left;
	left += BaseHardV(left);
	left += BaseHardBIU(left);
	left += BaseHardCHOICE(left);
	left += BaseHardCLIMBE(left);
	return left - T_preleft;
}

int CMap::Load_Model_Hardness2_Id4(int left)
{
	int T_preleft = left;
	left += BaseHardJump(left);
	left += BaseHardUUDD(left);
	left += BaseHardCHOICE(left);
	return left - T_preleft;
}


int CMap::Load_Model_Hardness2_Id5(int left)
{
	int T_preleft = left;
	left += BaseHardCLIMBE(left);
	left += BaseHardJump(left);
	left += BaseHardBIU(left);
	left += BaseHardCHOICE(left);
	return left - T_preleft;
}


int CMap::Load_Model_Hardness2_Id6(int left)
{
	int T_preleft = left;
	left += BaseHardW(left);
	left += BaseHardV(left);
	left += BaseHardV(left);
	return left - T_preleft;
}

int CMap::Load_Model_Hardness2_Id7(int left)
{
	int T_preleft = left;
	left += BaseHardJump(left);
	left += BaseHardJump(left);
	left += BaseHardBIU(left);
	left += BaseHardCLIMBE(left);
	return left - T_preleft;
}

int CMap::Load_Model_Hardness2_NICO(int left)
{
	int T_preleft = left;
	left += BaseHardNICOCONINI(left);
	left += BaseHardNICONICONI(left);
	left += BaseHardNICONINICO(left);
	return left - T_preleft;
}int CMap::Model_For_Fools(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 400;

	left += BaseHardNICONICONI(left);
	AddField(left, left + 1500, 500);
	AddStObstacle(left + 300, left + 400, 400, 500, SORANDUPDANGER);
	AddStObstacle(left + 700, left + 800, 400, 500, SORANDUPDANGER);
	AddStObstacle(left + 1400, left + 1500, 250, 350, SORANDUPDANGER);
	left += 1500;
	AddField(left, left + 300, 500);
	left += 300;
	AddField(left, left + 800, 500);
	AddStObstacle(left + 400, left + 600, 300, 500, SORANDUPDANGER);
	left += 1000;
	AddField(left, left + 800, 400);
	AddStObstacle(left + 400, left + 600, 200, 400, SORANDUPDANGER);
	return left - iOriginalLength;
}
int CMap::Load_Easy_Model(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 400;

	if ((rand() % 100) <= 80) left += Model_For_Fools(left);
	else (this->*m_pfnLoad_Model[0][Rand(0, g_cnt[0] - 1)])(left);
	//fprintf(stderr, "%d", rand() % 100);
	return left - iOriginalLength;
}
/*
*					SIGN FOR REMODELING: XQ
*/
int CMap::Load_Model_Hardness0_Id1(int left)
{
	//名前なし。
	int iOriginalLength = left;
	AddStObstacle(left + 200, left + 300, 400, 500, SOJUAN);
	AddField(left, left + 400, 500);
	left += 40;
	AddStObstacle(left + 250, left + 250 + 70, 0, 400, SOPILLAR);
	left += 600;
	AddField(left, left + 300, 500);
	left += 500;
	AddField(left, left + 300, 500);
	left += 40;
	AddStObstacle(left, left + 100, 0, 300, SOPILLAR);
	left += 500;
	AddField(left, left + 300, 500);
	AddStObstacle(left, left + 100, 400, 500, SOJUAN);
	left += 500;
	AddField(left, left + 300, 300);
	left += 500;
	AddField(left, left + 200, 400);
	left += 400;
	AddField(left, left + 200, 500);
	left += 400;
	AddField(left, left + 800, 550);
	AddStObstacle(left + 100, left + 300, 400, 550, SORANDUPDANGER);
	AddStObstacle(left + 500, left + 680, 0, 380, SOPILLAR);
	left += 900;
	return left - iOriginalLength;
}

/*
	Map Speed Range : 10 - 15
	Obstacles of SORANDUPDANGER should be changed into something else.
*/

int CMap::Load_Model_Hardness0_Id2(int left)
{
	int iOriginalLength = left;

	//Little up
	AddField(left, left + 400, 500);
	left += 400;
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 400;
	AddField(left, left + 100, 300);
	left += 300;
	AddStObstacle(left, left + 100, 500, 600, SORANDUPSAFE);
	left += 300;
	AddField(left, left + 300, 500);
	left += 300;

	//jumplands near
	AddField(left, left + 250, 400);
	left += 600;
	AddField(left, left + 300, 500);
	left += 600;
	AddField(left, left + 300, 500);
	AddStObstacle(left + 350, left + 450, 500, 600, SORANDUPDANGER);
	left += 600;
	AddField(left, left + 400, 350);
	left += 400;
	return left - iOriginalLength;
}


/*
	Map Speed Range : 10 - 15
	Obstacles of SORANDUPDANGER should be changed into something else.
*/

int CMap::Load_Model_Hardness0_Id3(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 400;

	//Deng Dua Lang
	AddField(left, left + 200, 500);
	left += 200;
	AddField(left + 200, left + 400, 400);
	left += 200;
	AddField(left + 400, left + 600, 300);
	AddStObstacle(left + 500, left + 600, 0, 220, SOPILLAR);
	left += 600;
	AddStObstacle(left + 100, left + 200, 550, 600, SOFIELD);
	left += 500;
	AddStObstacle(left, left + 100, 0, 250, SOPILLAR);
	AddField(left, left + 300, 400);
	left += 300;

	AddField(left, left + 400, 500);
	AddStObstacle(left + 200, left + 300, 400, 500, SOJUAN);
	left += 600;
	//jumplands near
	AddField(left, left + 250, 400);
	left += 600;
	AddField(left, left + 300, 500);
	left += 600;
	AddField(left, left + 300, 500);
	AddStObstacle(left + 350, left + 450, 500, 600, SORANDUPDANGER);
	left += 600;
	AddField(left, left + 400, 350);
	AddStObstacle(left, left + 100, 250, 350, SOJUAN);
	left += 400;
	return left - iOriginalLength;
}

int CMap::Load_Model_Hardness0_Id4(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 400;
	AddStObstacle(left, left + 100, 400, 500, SOJUAN);
	//jumplands far
	AddField(left, left + 200, 500);
	left += 600;
	AddField(left, left + 200, 300);
	left += 700;
	AddField(left, left + 200, 500);
	left += 700;
	AddField(left, left + 200, 550);
	left += 700;
	AddStObstacle(left, left + 100, 400, 500, SOJUAN);
	//Little up
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 400;
	AddField(left, left + 100, 300);
	left += 300;
	AddStObstacle(left, left + 100, 500, 600, SORANDUPSAFE);
	left += 300;
	AddField(left, left + 300, 500);
	AddStObstacle(left, left + 100, 400, 500, SOJUAN);
	left += 300;

	return left - iOriginalLength;
}




int CMap::Load_Model_Hardness0_Id5(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 400;

	//To Alice
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 350;
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 350;
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 350;
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 350;

	//Little up
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 400;
	AddField(left, left + 100, 300);
	left += 300;
	AddStObstacle(left, left + 100, 500, 600, SORANDUPSAFE);
	left += 300;
	AddField(left, left + 300, 500);
	left += 300;

	return left - iOriginalLength;
}

int CMap::Load_Model_Hardness0_Id6(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 400;

	//jumplands near
	AddStObstacle(left, left + 100, 300, 400, SOJUAN);
	AddField(left, left + 250, 400);
	left += 600;
	AddField(left, left + 300, 500);
	left += 600;
	AddStObstacle(left, left + 100, 400, 500, SOJUAN);
	AddField(left, left + 200, 500);
	AddStObstacle(left + 350, left + 450, 500, 600, SORANDUPDANGER);
	left += 600;
	AddField(left, left + 400, 350);
	left += 400;

	//Deng Dua Lang
	AddStObstacle(left, left + 100, 400, 500, SOJUAN);
	AddField(left, left + 200, 500);
	left += 200;
	AddField(left + 200, left + 400, 400);
	left += 200;
	AddField(left + 400, left + 600, 300);
	AddStObstacle(left + 500, left + 600, 0, 220, SOPILLAR);
	left += 600;
	AddStObstacle(left, left + 100, 400, 500, SOJUAN);
	AddStObstacle(left + 100, left + 200, 550, 600, SOFIELD);
	left += 500;
	AddStObstacle(left, left + 100, 0, 250, SOPILLAR);
	AddField(left, left + 300, 400);
	left += 500;
	return left - iOriginalLength;
}

int CMap::Load_Model_Hardness0_Id7(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 400;

	//Let them do math
	AddField(left, left + 250, 550);
	//AddStObstacle(left + 50, left + 150, 0, 300, SOPILLAR);
	left += 250;
	AddField(left, left + 250, 490);
	AddStObstacle(left + 50, left + 150, 0, 200, SOPILLAR);
	left += 250;
	AddField(left, left + 250, 530);
	AddStObstacle(left + 50, left + 150, 0, 170, SOPILLAR);
	left += 250;
	AddField(left, left + 250, 555);
	AddStObstacle(left + 50, left + 150, 0, 230, SOPILLAR);
	left += 500;
	AddField(left, left + 250, 540);
	AddStObstacle(left + 50, left + 150, 0, 230, SOPILLAR);
	left += 250;

	//Little up
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 400;
	AddField(left, left + 100, 300);
	left += 300;
	AddStObstacle(left, left + 100, 500, 600, SORANDUPSAFE);
	left += 300;
	AddField(left, left + 300, 500);
	left += 300;
	return left - iOriginalLength;
}



int CMap::Load_Model_Hardness0_Id8(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	AddStObstacle(left + 200, left + 300, 400, 500, SOJUAN);
	left += 500;

	//世界は変わった
	AddStObstacle(left, left + 1600, 50, 150, SORANDUPDANGER);
	AddField(left, left + 400, 300);
	AddStObstacle(left + 600, left + 800, 500, 600, SOFIELD);
	left += 800;
	AddField(left, left + 100, 500);
	AddField(left + 500, left + 600, 500);
	left += 700;


	//jumplands near
	AddField(left, left + 250, 400);
	left += 600;
	AddField(left, left + 300, 500);
	left += 600;
	AddField(left, left + 300, 500);
	AddStObstacle(left + 350, left + 450, 500, 600, SORANDUPDANGER);
	left += 600;
	AddField(left, left + 400, 350);
	left += 400;

	return left - iOriginalLength;
}

int CMap::Load_Model_Hardness0_Mixed(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 500;

	for (int i = 0; i < 2; ++i) left += (this->*m_pfnLoad_Model[0][rand() % 8])(left);
	return left - iOriginalLength;
}

int CMap::Load_Model_Hardness0_Mixed_Advanced(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 500;

	for (int i = 0; i < 2; ++i) left += Load_Model_Hardness0_Mixed(left);
	return left - iOriginalLength;
}

//#define SORANDUPDANGER 0


//Biobook up:safe, left:notsafe, bottom:notsafe

//phybook all notsafe

//#define MOCONFUSE 0
//#define MONICO 0


int CMap::Load_Model_Hardness1_Id0(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 500;

	//Wish for a better luck
	AddField(left, left + 500, 500);
	if (Rand(1, 100) <= 50) AddStObstacle(left, left + 100, 200, 300, SOJUAN);
	left += 900;
	if (Rand(1, 100) <= 60) AddStObstacle(left + 300, left + 400, 400, 500, SORANDUPSAFE);
	AddField(left, left + 400, 500);
	left += 800;
	if (Rand(1, 100) <= 80) AddStObstacle(left + 300, left + 400, 400, 500, SORANDUPSAFE);
	AddField(left, left + 400, 500);
	left += 800;
	if (Rand(1, 100) <= 90) AddStObstacle(left + 300, left + 400, 400, 500, SORANDUPSAFE);
	if (Rand(1, 100) <= 50) AddStObstacle(left, left + 100, 200, 300, SOJUAN);
	AddField(left, left + 400, 500);
	left += 800;
	if (Rand(1, 100) <= 90) AddStObstacle(left + 300, left + 400, 400, 500, SORANDUPSAFE);
	AddField(left, left + 400, 500);
	left += 800;
	if (Rand(1, 100) <= 80) AddStObstacle(left + 300, left + 400, 400, 500, SORANDUPSAFE);
	AddField(left, left + 400, 500);
	left += 800;
	if (Rand(1, 100) <= 75) AddStObstacle(left + 300, left + 400, 400, 500, SORANDUPSAFE);
	if (Rand(1, 100) <= 50) AddStObstacle(left, left + 100, 200, 300, SOJUAN);
	AddField(left, left + 400, 500);
	left += 800;


	//To Alice
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 350;
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 350;
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 350;
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 350;

	return left - iOriginalLength;
}


int CMap::Load_Model_Hardness1_Id1(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 500;
	// Let them do math
	AddField(left, left + 250, 550);
	//AddStObstacle(left + 50, left + 150, 0, 300, SOPILLAR);
	left += 250;
	AddField(left, left + 250, 490);
	AddStObstacle(left + 50, left + 150, 0, 200, SOPILLAR);
	left += 250;
	AddField(left, left + 250, 530);
	AddStObstacle(left + 50, left + 150, 0, 170, SOPILLAR);
	left += 250;
	AddField(left, left + 250, 555);
	AddStObstacle(left + 50, left + 150, 0, 230, SOPILLAR);
	left += 500;
	AddField(left, left + 250, 540);
	AddStObstacle(left + 50, left + 150, 0, 230, SOPILLAR);
	left += 250;


	//INTROVERT SHOULD DO
	AddField(left, left + 400, 500);
	left += 700;
	AddStObstacle(left, left + 100, 500, 600, SORANDUPSAFE);
	left += 300;
	AddStObstacle(left, left + 100, 500, 600, SORANDUPDANGER);
	left += 300;
	AddStObstacle(left, left + 100, 500, 600, SORANDUPSAFE);
	left += 300;
	AddStObstacle(left, left + 100, 500, 600, SORANDUPDANGER);
	left += 300;
	AddStObstacle(left, left + 100, 500, 600, SORANDUPSAFE);
	left += 300;
	AddStObstacle(left, left + 100, 500, 600, SORANDUPDANGER);
	left += 300;
	AddStObstacle(left, left + 100, 500, 600, SORANDUPSAFE);
	left += 300;
	AddStObstacle(left, left + 100, 500, 600, SORANDUPDANGER);
	left += 300;

	return left - iOriginalLength;
}

int CMap::Load_Model_Hardness1_Id2(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 500;

	//Ninokuni
	AddField(left, left + 400, 500);
	AddStObstacle(left + 200, left + 300, 150, 250, SOJUAN);
	left += 800;
	AddField(left, left + 200, 400);
	AddStObstacle(left + 300, left + 400, 300, 400, SORANDUPDANGER);
	AddStObstacle(left + 400, left + 500, 200, 300, SORANDUPSAFE);
	left += 200;
	AddField(left + 500, left + 600, 450);
	left += 800;
	AddField(left + 100, left + 300, 500);
	AddField(left + 500, left + 700, 300);
	AddStObstacle(left, left + 100, 200, 300, MOCONFUSE);
	left += 100;
	AddStObstacle(left + 600, left + 800, 0, 150, SORANDUPDANGER);
	left += 800;


	//Deng Dua Lang
	AddField(left, left + 200, 500);
	left += 200;
	AddField(left + 200, left + 400, 400);
	left += 200;
	AddField(left + 400, left + 600, 300);
	AddStObstacle(left + 500, left + 600, 0, 110, SOPILLAR);
	left += 600;
	AddStObstacle(left + 100, left + 200, 550, 600, SOFIELD);
	left += 500;
	AddStObstacle(left, left + 100, 0, 250, SOPILLAR);
	AddField(left, left + 300, 400);
	left += 300;

	//jumplands near
	AddField(left, left + 250, 400);
	left += 600;
	AddField(left, left + 300, 500);
	left += 600;
	AddField(left, left + 300, 500);
	AddStObstacle(left + 350, left + 450, 500, 600, SORANDUPDANGER);
	left += 600;
	AddField(left, left + 400, 350);
	left += 400;


	return left - iOriginalLength;
}

int CMap::Load_Model_Hardness1_Id3(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	AddStObstacle(left, left + 100, 200, 400, SOJUAN);
	left += 500;


	//To Alice
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 350;
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 350;
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	AddStObstacle(left + 100, left + 200, 200, 300, MONICO);
	left += 350;
	AddField(left, left + 200, 500);
	AddStObstacle(left + 100, left + 200, 0, 200, SOPILLAR);
	left += 350;
	AddField(left, left + 400, 500);
	left += 600;
	AddField(left, left + 250, 550);


	//Let them do math
	//AddStObstacle(left + 50, left + 150, 0, 300, SOPILLAR);
	left += 250;
	AddField(left, left + 250, 490);
	AddStObstacle(left, left + 100, 200, 400, SOJUAN);
	AddStObstacle(left + 50, left + 150, 0, 200, SOPILLAR);
	left += 250;
	AddField(left, left + 250, 530);
	AddStObstacle(left + 50, left + 150, 0, 170, SOPILLAR);
	left += 250;
	AddField(left, left + 250, 555);
	AddStObstacle(left + 50, left + 150, 0, 230, SOPILLAR);
	left += 500;
	AddField(left, left + 250, 540);
	AddStObstacle(left + 50, left + 150, 0, 230, SOPILLAR);
	left += 250;
	AddStObstacle(left, left + 100, 200, 400, SOJUAN);
	AddField(left, left + 400, 500);
	left += 600;
	//世界は変わった
	AddStObstacle(left, left + 1600, 50, 150, SORANDUPDANGER);
	AddField(left, left + 400, 300);
	AddStObstacle(left + 600, left + 800, 500, 600, SOFIELD);
	left += 800;
	AddField(left, left + 100, 500);
	AddField(left + 500, left + 600, 500);
	left += 700;
	AddField(left, left + 400, 500);
	left += 600;

	//Wish for a better luck, reduced version.
	AddField(left, left + 500, 500);
	if (Rand(1, 100) <= 50) AddStObstacle(left, left + 100, 200, 400, SOJUAN);
	left += 900;
	if (Rand(1, 100) <= 60) AddStObstacle(left + 300, left + 400, 400, 500, SORANDUPSAFE);
	AddField(left, left + 400, 500);
	left += 800;
	if (Rand(1, 100) <= 80) AddStObstacle(left + 300, left + 400, 400, 500, SORANDUPSAFE);
	AddField(left, left + 400, 500);
	left += 800;
	AddStObstacle(left, left + 100, 200, 400, SOJUAN);
	return left - iOriginalLength;
}

int CMap::Load_Model_Hardness1_Id4(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 500;


	//jumplands near advanced
	AddField(left, left + 250, 400);
	left += 600;
	AddField(left, left + 300, 500);
	AddMoObstacle(left + 200, left + 300, 400, 500, MOCONFUSE);
	left += 600;
	AddField(left, left + 300, 500);
	AddStObstacle(left + 350, left + 450, 500, 600, SORANDUPDANGER);
	left += 600;
	AddField(left, left + 400, 350);
	AddStObstacle(left + 200, left + 300, 250, 350, MONICO);
	left += 400;
	AddField(left, left + 200, 400);
	left += 600;
	AddField(left, left + 200, 500);
	left += 600;
	AddField(left, left + 200, 500);
	AddStObstacle(left + 350, left + 450, 500, 600, SORANDUPDANGER);
	AddStObstacle(left + 200, left + 300, 400, 500, MOCONFUSE);
	left += 600;
	AddField(left, left + 400, 350);
	left += 400;

	//jumplands near advanced
	AddField(left, left + 250, 400);
	left += 600;
	AddField(left, left + 300, 500);
	AddMoObstacle(left + 200, left + 300, 400, 500, MONICO);
	left += 600;
	AddField(left, left + 300, 500);
	AddStObstacle(left + 200, left + 300, 400, 500, MOCONFUSE);
	AddStObstacle(left + 350, left + 450, 500, 600, SORANDUPDANGER);
	left += 600;
	AddField(left, left + 400, 350);
	AddStObstacle(left + 200, left + 300, 250, 350, MONICO);
	left += 400;

	return left - iOriginalLength;
}

int CMap::Load_Model_Hardness1_Id5(int left)
{
	int iOriginalLength = left;
	AddField(left, left + 400, 500);
	left += 400;

	left += Load_Model_Hardness1_Id0(left);

	//Ninokuni
	AddField(left, left + 400, 500);
	AddStObstacle(left + 200, left + 300, 150, 250, SOJUAN);
	left += 800;
	AddField(left, left + 200, 400);
	AddStObstacle(left + 300, left + 400, 300, 400, SORANDUPDANGER);
	AddStObstacle(left + 400, left + 500, 200, 300, SORANDUPSAFE);
	left += 200;
	AddField(left + 500, left + 600, 450);
	left += 800;
	AddField(left + 100, left + 300, 500);
	AddField(left + 500, left + 700, 300);
	AddStObstacle(left, left + 100, 200, 300, MONICO);
	left += 100;
	AddStObstacle(left + 600, left + 800, 0, 150, SORANDUPDANGER);
	left += 800;
	return left - iOriginalLength;
}