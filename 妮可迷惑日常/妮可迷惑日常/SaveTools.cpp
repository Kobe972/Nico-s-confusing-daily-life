#include "SaveTools.h"
#include <io.h>

const char* CSaveTools::filename = "save.data";
/*
* bool CSaveTools::LoadSave(Save& output)
* 将存档信息写入output
* 发生错误返回false， 否则返回true
*/
bool CSaveTools::LoadSave(Save& output)
{
    FILE *fp;
    int fd = 0;

    if ((fp = fopen(CSaveTools::filename, "rb")) == NULL) {
        if ((fp = fopen(CSaveTools::filename, "wb")) == NULL) return false;
        fclose(fp);
        if ((fp = fopen(CSaveTools::filename, "rb")) == NULL) return false;
    }

    int n_read = fread(&output, sizeof(Save), 1, fp);
    if (!n_read) {
        memset(&output, 0, sizeof(output));
        output.isActive = SIGN_ACTIVE;
        output.bSkinUnlocked[0] = true;
    }
    if (fp) fclose(fp);
    return true;
}
/*
* bool CSaveTools::SaveCurrent(int* iObstacleFailure, int* iObstacleSuccess, bool* bSkinUnlocked, bool* bAchievements)
* 参数分别为相应的增加量
* 注意两个bool数组也是增加量，可以只传递增加获得的
* 发生错误返回false,否则返回true
*/
/*bool CSaveTools::SaveCurrent(int* iObstacleFailure, int* iObstacleSuccess, bool* bSkinUnlocked, bool* bAchievements)
{
    Save tmp;
    FILE* fp;
    if (!LoadSave(tmp)) return false;
    if (tmp.isActive != SIGN_ACTIVE) return false;
    for (int i = 0; i < OBSTACLE_NUM; ++i)
        tmp.iObstacleFailure[i] += iObstacleFailure[i], 
        tmp.iObstacleSuccess[i] += iObstacleSuccess[i];
    for (int i = 0; i < SKIN_NUM; ++i)
        tmp.bSkinUnlocked[i] = (bSkinUnlocked[i] || tmp.bSkinUnlocked[i]);
    for (int i = 0; i < ACHIEVEMENT_NUM; ++i)
        tmp.bAchievements[i] = (bAchievements[i] || tmp.bAchievements[i]);


    if ((fp = fopen(CSaveTools::filename, "wb")) == NULL) return false;
    fwrite(&tmp, sizeof(Save), 1, fp);
    if (fp) fclose(fp);
    return true;
}*/

bool CSaveTools::SaveDirectly(Save tmp)
{
    FILE* fp;
    if ((fp = fopen(CSaveTools::filename, "wb")) == NULL) return false;
    fwrite(&tmp, sizeof(Save), 1, fp);
    if (fp) fclose(fp);
    return true;
}





/*
* 使用实例
void debug(void)
{
    Save data;
    if (!CSaveTools::LoadSave(data))
        MessageBox(NULL, "Bugs occur.", "Attention", MB_OK);

    int a[100] = { 1, 1, 1, 1, 1 };
    if(!(CSaveTools::SaveCurrent(a, a, (bool *)a, (bool*)a)))
        MessageBox(NULL, "Bugs occur.Second place.", "Attention", MB_OK);

    memset(&data, 0, sizeof(Save));
    if (!CSaveTools::LoadSave(data))
        MessageBox(NULL, "Bugs occur.Third.", "Attention", MB_OK);
}
*/