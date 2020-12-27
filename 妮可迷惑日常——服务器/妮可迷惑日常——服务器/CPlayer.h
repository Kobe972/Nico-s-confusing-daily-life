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

//player state
#define LOBBY 0
#define WAITING 1 //waiting to play
#define PLAYING 2
struct Msg
{
	int ID=0;
	int num=0;
	char string1[50] = { 0 };
	char string2[50] = { 0 };
};
void MsgToString(Msg message, char* ans);//把Msg型数据表示成字符串格式，以便发送
void StringToMsg(Msg* message, char* str);//把字符串解码回Msg型数据格式
class CPlayer
{
public:
	char name[20]="Tourist";
	int Num_Competition=0;
	int GPA=0;
	SOCKET conn;//用于和客户端交流的套接字
	bool connected = false; //conn是否连上客户端
	sockaddr_in clientAddr; //客户端地址信息（只是函数参数需要，暂无实际用处）
	int state=LOBBY;//该玩家状态，默认休息
	int connect(SOCKET listening_sock);//检测conn有没有客户端可连，若有则连上
	int receive(Msg* message);//接收信息并存入message，接受成功返回1，否则返回0
	int send_message(Msg* message);//发送message信息给客户端
	void check();//判断连接是否断开，如断开connected置零
};
struct Player_Information //玩家信息，数据库中存的数据格式
{
	char name[20];
	char passwd[20];
	int Num_Competition; //总共打过几次比赛
	int GPA;
};
int comp(const void* p1, const void* p2); //比较函数，给Player_Information型数据按GPA大小排序用
bool SendData(SOCKET m_hSocket, const char* buf, int buf_length);
