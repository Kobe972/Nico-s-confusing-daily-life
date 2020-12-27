#include"CPlayer.h"
using namespace std;
CPlayer player[20];
SOCKET serverSocket;
void Set_GPA(int player_ID, int GPA);//通知第player_ID号玩家更新GPA
void Write_GPA(int player_ID, int GPA); //将第player_ID号玩家的GPA写入服务器数据库，注意每调用一次函数会顺便使该玩家的Num_Competition加一
void Join(int person); //第person号玩家状态设为等待游戏开始
void WaitForJoining(); //等待玩家加入
int Get_SurvNum(); //获得目前存活玩家数
void Send_Rank(int dest); //发送GPA总排名前十位给player[dest]
void Log(char* name, char* passwd,int ID); //palyer[ID]登录
void Reg(char* name, char* passwd,int ID); //player[ID]注册，之前它是游客，不能参与正式游戏
void Send_Map();//注意只是将消息加入队列，并不发送
void Process(Msg message,int src); //处理消息
int state=0;//0表示等待玩家加入游戏，1表示已经开始游戏
clock_t clk;
class MsgList
{
public:
	queue<Msg>messages; //队列中的消息
	queue<int> Dest; //各消息要发送给哪个玩家，如Dest=1表示发给player[1]
	void add(Msg message,int dest); //向消息列表添加消息
	void Send(); //发送列表中所有消息
} msglist;
int main()
{
	int mode = 1;
	clk = clock();
	sockaddr_in addrSrv;
	WSADATA wsadata;
	//初始化套接字库
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	ioctlsocket(serverSocket, FIONBIO, (u_long FAR*) & mode);
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8000);
	bind(serverSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	listen(serverSocket, 100);
	while (1)
	{
		system("cls");
		for (int i = 0; i < 20; i++) //依次检测每个玩家，看是否连接
		{
			player[i].check();
			if (!player[i].connected) player[i].connect(serverSocket);
		}
		Msg mes;
		for (int i = 0; i < 20; i++)
		{
			player[i].check();
			if (player[i].connected)
			{
				if (player[i].receive(&mes)) Process(mes,i); //接受玩家信息并进行处理
				msglist.Send(); //将消息列表中的信息发送
				//服务器界面显示玩家基本信息
				printf("player[%d].name=%s\nplayer[%d].GPA=%d\nplayer[%d].Num_Competition=%d\n", i, player[i].name, i, player[i].GPA,i,player[i].Num_Competition);
				switch (player[i].state)
				{
				case LOBBY:
					printf("player[%d].state=LOBBY\n", i);
					break;
				case WAITING:
					printf("player[%d].state=WAITING\n", i);
					break;
				case PLAYING:
					printf("player[%d].state=PLAYING\n", i);
					break;
				default:
					break;
				}
			}
		}
		if (state == 0) WaitForJoining(); //等待玩家开始双人模式游戏
	}
	closesocket(serverSocket);
	return 0;
}

void MsgList::add(Msg message,int dest)
{
	messages.push(message); 
	Dest.push(dest);
}

void MsgList::Send()
{
	while(!Dest.empty())
	{
		player[Dest.front()].check();
		if (player[Dest.front()].connected)
		{
			player[Dest.front()].send_message(&messages.front());
		}
		messages.pop();
		Dest.pop();
	}
}

void Set_GPA(int player_ID, int GPA)
{
	Msg mes;
	mes.ID = SET_GPA;
	mes.num = GPA;
	msglist.add(mes, player_ID);
	return;
}

void Write_GPA(int player_ID, int GPA)
{
	//思路：在OutFile中写更新后的所有玩家数据，并删除原数据文件，将新文件重命名为原来数据文件名
	FILE* InFile;//原数据文件
	FILE* OutFile;//新数据文件
	InFile = fopen("data.dat", "rb"); 
	OutFile = fopen("tmp.dat", "wb"); 
	Player_Information buf;
	while (!feof(InFile) && fread(&buf, sizeof(buf), 1, InFile))
	{
		if (strcmp(buf.name, player[player_ID].name) != 0)//其他玩家信息照抄
		{
			fwrite(&buf, sizeof(buf), 1, OutFile);
			continue;
		}
		//重写该玩家信息
		buf.GPA = GPA;
		buf.Num_Competition++;//注意每调用一次函数会顺便使该玩家的Num_Competition加一
		fwrite(&buf, sizeof(buf), 1, OutFile);
	}
	fclose(InFile);
	fclose(OutFile);
	//下面进行文件替换
	remove("data.dat");//删除旧文件
	rename("tmp.dat", "data.dat");//重命名新文件
}

void Join(int person)
{
	player[person].state = WAITING;
	return;
}

void WaitForJoining()
{
	int waiting_num=0; //统计有多少玩家在等待游戏开始
	static int waiting_num_pre=0;//上一次调用函数时等待游戏开始的人数
	int waiting_ID[20]; //记录每个state=WAITING的玩家的ID
	for (int i = 0; i < 20; i++) //检测每个玩家的状态并进行一些量的赋值
	{
		player[i].check();
		if (player[i].connected && player[i].state == WAITING)
		{
			waiting_ID[waiting_num] = i;
			waiting_num++;
		}
	}
	if (waiting_num_pre == 0 && waiting_num > 0) //从没人等待到有人等待，计时器开始计时
	{
		clk = clock();
	}
	waiting_num_pre = waiting_num;
	if (clock() - clk < 30000) return; //未到时
	if (waiting_num <= 1) return; //人太少
	//如果运行到这，说明游戏可以开始了
	state = 1;//游戏模式
	Send_Map(); //发送地图信息
	Msg mes;
	mes.ID = BEGIN_GAME;
	for (int i = 0; i < waiting_num; i++) //通知每个玩家游戏开始
	{
		player[i].state = PLAYING;
		player[waiting_ID[i]].check();
		if (player[i].connected) msglist.add(mes, i);
	}
	mes.ID = CLEAR_RANK;
	for (int i = 0; i < waiting_num; i++) //通知每个玩家清除排名列表
	{
		player[waiting_ID[i]].check();
		if (player[i].connected) msglist.add(mes, i);
	}
	mes.ID = NUM_PLAYING;
	for (int i = 0; i < waiting_num; i++) //通知每个玩家清除排名列表
	{
		mes.num = waiting_num;
		player[waiting_ID[i]].check();
		if (player[i].connected) msglist.add(mes, i);
	}
}

int Get_SurvNum()
{
	int num = 0;
	for (int i = 0; i < 20; i++)
	{
		player[i].check();
		if (player[i].connected && player[i].state == PLAYING) num++;
	}
	return num;
}

void Send_Rank(int dest)
{
	Msg mes;
	mes.ID = CLEAR_RANK;
	msglist.add(mes, dest);//通知客户端清空排名列表
	Player_Information info[200]; //info为information的缩写，记录数据库200个玩家的信息（因此要求数据库不能多于200玩家）
	FILE* InFile;
	InFile = fopen("data.dat", "rb");
	int Num_of_Players = 0; //数据库中一共存了多少玩家信息
	while (!feof(InFile) && fread(&info[Num_of_Players], sizeof(info[Num_of_Players]), 1, InFile)&&Num_of_Players<200)
	{
		Num_of_Players++;
	}
	fclose(InFile);
	qsort(info, Num_of_Players, sizeof(Player_Information), comp); //info按GPA排序
	mes.ID = RANK_ITEM;
	for (int i = 0; i < min(10,Num_of_Players); i++) //选出至多前十个发送过去
	{
		mes.num = info[i].GPA;
		strcpy(mes.string1, info[i].name);
		msglist.add(mes, dest);
	}
	return;
}

void Log(char* name, char* passwd,int ID)
{
	Msg mes;
	Player_Information info;
	bool success = false;
	FILE* InFile; //该文件用来查找用户信息
	InFile = fopen("data.dat", "rb");
	while (!feof(InFile) && fread(&info, sizeof(info), 1, InFile))
	{
		if (strcmp(info.name, name) == 0 && strcmp(info.passwd, passwd) == 0)
		{
			success = 1;
			break;
		}
	}
	fclose(InFile);
	mes.ID = LOG_RESULT;
	if (!success)//登录失败
	{
		mes.num = 0;
		msglist.add(mes, ID);
		return;
	}
	//登录成功
	mes.num = 1;
	msglist.add(mes, ID);
	//发送GPA
	Set_GPA(ID, info.GPA);
	//初始化玩家信息
	strcpy(player[ID].name, info.name);
	player[ID].GPA = info.GPA;
	player[ID].Num_Competition = info.Num_Competition;
	player[ID].state = LOBBY;
	return;
}

void Reg(char* name, char* passwd, int ID)
{
	Msg mes;
	Player_Information info;
	bool success = true;
	FILE* InFile;
	InFile = fopen("data.dat", "ab+");
	int Num_of_Players = 0;
	while (!feof(InFile) && fread(&info, sizeof(info), 1, InFile))
	{
		if (strcmp(info.name, name) == 0)
		{
			success = false;
			break;
		}
	}
	mes.ID = REG_RESULT;
	if (!success)//注册失败
	{
		mes.num = 0;
		msglist.add(mes, ID);
		return;
	}
	mes.num = 1;
	msglist.add(mes, ID);
	info.GPA = info.Num_Competition = 0;
	strcpy(info.name, name);
	strcpy(info.passwd, passwd);
	fwrite(&info, sizeof(Player_Information), 1, InFile);//写入新数据
	fclose(InFile);
	return;
}

void Send_Map()
{
}

void Process(Msg message, int src)
{
	Msg mes;
	switch (message.ID)
	{
	case JOIN:
		Join(src);
		break;
	case END_GAME:
		//重写GPA，改变玩家参数
		player[src].GPA = (message.num + player[src].GPA* player[src].Num_Competition) / (player[src].Num_Competition + 1);
		player[src].Num_Competition++;
		player[src].state = LOBBY;
		Set_GPA(src, player[src].GPA);
		Write_GPA(src, player[src].GPA);
		if (Get_SurvNum() == 0) state = 0;//检查是否人已死光
		//向所有玩家发送绩点信息
		mes.ID = RANK_ITEM;
		mes.num = message.num;
		strcpy(mes.string1, player[src].name);
		for (int i = 0; i < 20; i++)
		{
			if (player[i].connected) msglist.add(mes, i);
		}
		break;
	case LOG:
		Log(message.string1, message.string2, src);
		break;
	case REG:
		Reg(message.string1, message.string2, src);
		break;
	case RANK_RQ:
		Send_Rank(src);
		break;
	default:
		break;
	}
}
