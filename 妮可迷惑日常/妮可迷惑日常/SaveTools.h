#pragma once
#include "pch.h"

#define MAX_NUM 100
#define SIGN_ACTIVE 0xf0f0f0f0
#define OBSTACLE_NUM 100
#define SKIN_NUM 100
#define ACHIEVEMENT_NUM 100

typedef struct {
	int iStObstacleFailure[MAX_NUM];
	int iStObstacleSuccess[MAX_NUM];
	int iMoObstacleFailure[MAX_NUM];
	int iMoObstacleSuccess[MAX_NUM];
	bool bSkinUnlocked[MAX_NUM];
	bool bAchievements[MAX_NUM];
	int MapLength;
	int isActive;
} Save;

class CSaveTools
{
public:
	static const char* filename;

	static bool LoadSave(Save& output);
	//static bool SaveCurrent(int* iObstacleFailure, int* iObstacleSuccess, bool* bSkinUnlocked,
	//	bool* bAchievements);
	static bool SaveDirectly(Save tmp);
};

