#include "game.h"

inline void CGame::SetGameState(EGameState eGameStateCurrent)
{
	eGameState = eGameStateCurrent;
}

inline void CGame::SetWindowHandle(HWND hwnd)
{
	m_hWnd = hwnd;
}
