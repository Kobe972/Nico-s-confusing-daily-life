#pragma once
#include<Windows.h>
#include<string>
#include<ctime>
#include<queue>
//ID of structure MSG
#define SET_GPA 0 //设置GPA（服务器给客户端）
#define JOIN 1 //加入比赛（客户端给服务器）
#define END_GAME 2 //游戏结束，客户端发给服务器表示玩家死亡，服务器发给客户端表示比赛结束（双向均行）
#define RANK_ITEM 3 //表示该消息是排名列表中的一项（服务器给客户端）
#define CLEAR_RANK 4 //清除排名列表（服务器给客户端）
//说明一下，客户端含有一个数据结构，包括排名列表里所有的人名和GPA，当收到RANK_ITEM型的数据后，在列表里添加新信息；收到CLEAR_RANK后，清空列表。当需要显示排名时，直接打印此列表即可
#define LOG 5 //登录（客户端给服务器）
#define LOG_RESULT 6 //登录结果（服务器给客户端），num=0表示失败，num=1表示成功，num的含义见Msg结构的定义
#define REG 7 //注册（客户端给服务器）
#define REG_RESULT 8 //注册结果（服务器给客户端）
#define RANK_RQ 9 //请求获得排名列表（客户端给服务器）
#define BEGIN_GAME 10 //开始游戏（服务器给客户端）
#define MAPMSG 11 //地图信息（服务器给客户端）
#define NUM_PLAYING 12
#define DISCONNECT 13

//player state
#define LOBBY 0
#define WAITING 1 //waiting to play
#define PLAYING 2
#pragma comment(lib, "ws2_32.lib") 
struct Msg
{
	int ID;
	int num;
	char string1[50] = "?";
	char string2[50] = "?";
};

DWORD WINAPI CreateCilent(LPVOID);
bool SendData(const char* buf, int buf_length);
int send_message(Msg* message);//发送message信息给服务端
void Send(); //发送列表中所有消息

void MsgToString(Msg message, char* ans);//把Msg型数据表示成字符串格式，以便发送
Msg StringToMsg(char* str);//把字符串解码回Msg型数据格式
