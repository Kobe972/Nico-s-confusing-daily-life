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
int send_message(Msg* message);//����message��Ϣ�������
void Send(); //�����б���������Ϣ

void MsgToString(Msg message, char* ans);//��Msg�����ݱ�ʾ���ַ�����ʽ���Ա㷢��
Msg StringToMsg(char* str);//���ַ��������Msg�����ݸ�ʽ
