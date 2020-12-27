#pragma once
#include<Windows.h>
#include<string>
#include<ctime>
#include<queue>
//ID of structure MSG
#define SET_GPA 0 //����GPA�����������ͻ��ˣ�
#define JOIN 1 //����������ͻ��˸���������
#define END_GAME 2 //��Ϸ�������ͻ��˷�����������ʾ��������������������ͻ��˱�ʾ����������˫����У�
#define RANK_ITEM 3 //��ʾ����Ϣ�������б��е�һ����������ͻ��ˣ�
#define CLEAR_RANK 4 //��������б����������ͻ��ˣ�
//˵��һ�£��ͻ��˺���һ�����ݽṹ�����������б������е�������GPA�����յ�RANK_ITEM�͵����ݺ����б����������Ϣ���յ�CLEAR_RANK������б�����Ҫ��ʾ����ʱ��ֱ�Ӵ�ӡ���б���
#define LOG 5 //��¼���ͻ��˸���������
#define LOG_RESULT 6 //��¼��������������ͻ��ˣ���num=0��ʾʧ�ܣ�num=1��ʾ�ɹ���num�ĺ����Msg�ṹ�Ķ���
#define REG 7 //ע�ᣨ�ͻ��˸���������
#define REG_RESULT 8 //ע���������������ͻ��ˣ�
#define RANK_RQ 9 //�����������б��ͻ��˸���������
#define BEGIN_GAME 10 //��ʼ��Ϸ�����������ͻ��ˣ�
#define MAPMSG 11 //��ͼ��Ϣ�����������ͻ��ˣ�

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
void MsgToString(Msg message, char* ans);//��Msg�����ݱ�ʾ���ַ�����ʽ���Ա㷢��
void StringToMsg(Msg* message, char* str);//���ַ��������Msg�����ݸ�ʽ
class CPlayer
{
public:
	char name[20]="Tourist";
	int Num_Competition=0;
	int GPA=0;
	SOCKET conn;//���ںͿͻ��˽������׽���
	bool connected = false; //conn�Ƿ����Ͽͻ���
	sockaddr_in clientAddr; //�ͻ��˵�ַ��Ϣ��ֻ�Ǻ���������Ҫ������ʵ���ô���
	int state=LOBBY;//�����״̬��Ĭ����Ϣ
	int connect(SOCKET listening_sock);//���conn��û�пͻ��˿���������������
	int receive(Msg* message);//������Ϣ������message�����ܳɹ�����1�����򷵻�0
	int send_message(Msg* message);//����message��Ϣ���ͻ���
	void check();//�ж������Ƿ�Ͽ�����Ͽ�connected����
};
struct Player_Information //�����Ϣ�����ݿ��д�����ݸ�ʽ
{
	char name[20];
	char passwd[20];
	int Num_Competition; //�ܹ�������α���
	int GPA;
};
int comp(const void* p1, const void* p2); //�ȽϺ�������Player_Information�����ݰ�GPA��С������
bool SendData(SOCKET m_hSocket, const char* buf, int buf_length);
