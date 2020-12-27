#include"CPlayer.h"
using namespace std;
CPlayer player[20];
SOCKET serverSocket;
void Set_GPA(int player_ID, int GPA);//֪ͨ��player_ID����Ҹ���GPA
void Write_GPA(int player_ID, int GPA); //����player_ID����ҵ�GPAд����������ݿ⣬ע��ÿ����һ�κ�����˳��ʹ����ҵ�Num_Competition��һ
void Join(int person); //��person�����״̬��Ϊ�ȴ���Ϸ��ʼ
void WaitForJoining(); //�ȴ���Ҽ���
int Get_SurvNum(); //���Ŀǰ��������
void Send_Rank(int dest); //����GPA������ǰʮλ��player[dest]
void Log(char* name, char* passwd,int ID); //palyer[ID]��¼
void Reg(char* name, char* passwd,int ID); //player[ID]ע�ᣬ֮ǰ�����οͣ����ܲ�����ʽ��Ϸ
void Send_Map();//ע��ֻ�ǽ���Ϣ������У���������
void Process(Msg message,int src); //������Ϣ
int state=0;//0��ʾ�ȴ���Ҽ�����Ϸ��1��ʾ�Ѿ���ʼ��Ϸ
clock_t clk;
class MsgList
{
public:
	queue<Msg>messages; //�����е���Ϣ
	queue<int> Dest; //����ϢҪ���͸��ĸ���ң���Dest=1��ʾ����player[1]
	void add(Msg message,int dest); //����Ϣ�б������Ϣ
	void Send(); //�����б���������Ϣ
} msglist;
int main()
{
	int mode = 1;
	clk = clock();
	sockaddr_in addrSrv;
	WSADATA wsadata;
	//��ʼ���׽��ֿ�
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
		for (int i = 0; i < 20; i++) //���μ��ÿ����ң����Ƿ�����
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
				if (player[i].receive(&mes)) Process(mes,i); //���������Ϣ�����д���
				msglist.Send(); //����Ϣ�б��е���Ϣ����
				//������������ʾ��һ�����Ϣ
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
		if (state == 0) WaitForJoining(); //�ȴ���ҿ�ʼ˫��ģʽ��Ϸ
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
	//˼·����OutFile��д���º������������ݣ���ɾ��ԭ�����ļ��������ļ�������Ϊԭ�������ļ���
	FILE* InFile;//ԭ�����ļ�
	FILE* OutFile;//�������ļ�
	InFile = fopen("data.dat", "rb"); 
	OutFile = fopen("tmp.dat", "wb"); 
	Player_Information buf;
	while (!feof(InFile) && fread(&buf, sizeof(buf), 1, InFile))
	{
		if (strcmp(buf.name, player[player_ID].name) != 0)//���������Ϣ�ճ�
		{
			fwrite(&buf, sizeof(buf), 1, OutFile);
			continue;
		}
		//��д�������Ϣ
		buf.GPA = GPA;
		buf.Num_Competition++;//ע��ÿ����һ�κ�����˳��ʹ����ҵ�Num_Competition��һ
		fwrite(&buf, sizeof(buf), 1, OutFile);
	}
	fclose(InFile);
	fclose(OutFile);
	//��������ļ��滻
	remove("data.dat");//ɾ�����ļ�
	rename("tmp.dat", "data.dat");//���������ļ�
}

void Join(int person)
{
	player[person].state = WAITING;
	return;
}

void WaitForJoining()
{
	int waiting_num=0; //ͳ���ж�������ڵȴ���Ϸ��ʼ
	static int waiting_num_pre=0;//��һ�ε��ú���ʱ�ȴ���Ϸ��ʼ������
	int waiting_ID[20]; //��¼ÿ��state=WAITING����ҵ�ID
	for (int i = 0; i < 20; i++) //���ÿ����ҵ�״̬������һЩ���ĸ�ֵ
	{
		player[i].check();
		if (player[i].connected && player[i].state == WAITING)
		{
			waiting_ID[waiting_num] = i;
			waiting_num++;
		}
	}
	if (waiting_num_pre == 0 && waiting_num > 0) //��û�˵ȴ������˵ȴ�����ʱ����ʼ��ʱ
	{
		clk = clock();
	}
	waiting_num_pre = waiting_num;
	if (clock() - clk < 30000) return; //δ��ʱ
	if (waiting_num <= 1) return; //��̫��
	//������е��⣬˵����Ϸ���Կ�ʼ��
	state = 1;//��Ϸģʽ
	Send_Map(); //���͵�ͼ��Ϣ
	Msg mes;
	mes.ID = BEGIN_GAME;
	for (int i = 0; i < waiting_num; i++) //֪ͨÿ�������Ϸ��ʼ
	{
		player[i].state = PLAYING;
		player[waiting_ID[i]].check();
		if (player[i].connected) msglist.add(mes, i);
	}
	mes.ID = CLEAR_RANK;
	for (int i = 0; i < waiting_num; i++) //֪ͨÿ�������������б�
	{
		player[waiting_ID[i]].check();
		if (player[i].connected) msglist.add(mes, i);
	}
	mes.ID = NUM_PLAYING;
	for (int i = 0; i < waiting_num; i++) //֪ͨÿ�������������б�
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
	msglist.add(mes, dest);//֪ͨ�ͻ�����������б�
	Player_Information info[200]; //infoΪinformation����д����¼���ݿ�200����ҵ���Ϣ�����Ҫ�����ݿⲻ�ܶ���200��ң�
	FILE* InFile;
	InFile = fopen("data.dat", "rb");
	int Num_of_Players = 0; //���ݿ���һ�����˶��������Ϣ
	while (!feof(InFile) && fread(&info[Num_of_Players], sizeof(info[Num_of_Players]), 1, InFile)&&Num_of_Players<200)
	{
		Num_of_Players++;
	}
	fclose(InFile);
	qsort(info, Num_of_Players, sizeof(Player_Information), comp); //info��GPA����
	mes.ID = RANK_ITEM;
	for (int i = 0; i < min(10,Num_of_Players); i++) //ѡ������ǰʮ�����͹�ȥ
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
	FILE* InFile; //���ļ����������û���Ϣ
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
	if (!success)//��¼ʧ��
	{
		mes.num = 0;
		msglist.add(mes, ID);
		return;
	}
	//��¼�ɹ�
	mes.num = 1;
	msglist.add(mes, ID);
	//����GPA
	Set_GPA(ID, info.GPA);
	//��ʼ�������Ϣ
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
	if (!success)//ע��ʧ��
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
	fwrite(&info, sizeof(Player_Information), 1, InFile);//д��������
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
		//��дGPA���ı���Ҳ���
		player[src].GPA = (message.num + player[src].GPA* player[src].Num_Competition) / (player[src].Num_Competition + 1);
		player[src].Num_Competition++;
		player[src].state = LOBBY;
		Set_GPA(src, player[src].GPA);
		Write_GPA(src, player[src].GPA);
		if (Get_SurvNum() == 0) state = 0;//����Ƿ���������
		//��������ҷ��ͼ�����Ϣ
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
