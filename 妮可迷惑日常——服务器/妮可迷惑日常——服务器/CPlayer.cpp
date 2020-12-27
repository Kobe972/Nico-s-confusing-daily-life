#include "CPlayer.h"
bool SendData(SOCKET m_hSocket, const char* buf, int buf_length)
{
	//�ѷ��͵��ֽ���Ŀ
	int sent_bytes = 0;
	int ret = 0;
	while (true)
	{
		ret = send(m_hSocket, buf + sent_bytes, buf_length - sent_bytes, 0);
		if (ret == -1)
		{
			if (errno == EWOULDBLOCK)
			{
				//�Ͻ����������������������ȥ��Ӧ�û�����δ����ȥ�����ݣ��������
				break;
			}
			else if (errno == EINTR)
				continue;
			else
				return true;
		}
		else if (ret == 0)
		{
			return true;
		}

		sent_bytes += ret;
		if (sent_bytes == buf_length)
			break;

		//���Խ��� CPU ��ʹ����
		Sleep(1);
	}

	return false;
}
int CPlayer::connect(SOCKET listening_sock)
{
	int len = sizeof(SOCKADDR);
	conn = accept(listening_sock, (SOCKADDR*)&clientAddr, &len);
	if (conn != INVALID_SOCKET)
	{
		connected = true;
		return 1;
	}
	return 0;
}

int CPlayer::receive(Msg* message)
{
	char buf[50];
	if(connected == false) return 0; 
	if (recv(conn, buf, sizeof(buf), 0) > 0)
	{
		StringToMsg(message, buf);
		return 1;
	}
	return 0;
}

int CPlayer::send_message(Msg* message)
{
	char buf[50];
	MsgToString(*message, buf);
	if (connected == false) return 0;
	if(SendData(conn, buf, sizeof(buf))) return 1;
	return 0;
}

void MsgToString(Msg message, char* ans)
{
	std::string tmp;
	tmp = std::to_string(message.ID) + " " + std::to_string(message.num) + " ";
	strcpy(ans, tmp.c_str());
	strcat(ans, message.string1);
	strcat(ans, " ");
	strcat(ans, message.string2);
	return;
}

void StringToMsg(Msg* message, char* str)
{
	char* tmp;
	tmp = strtok(str, " ");
	message->ID = atoi(tmp);
	tmp = strtok(NULL, " ");
	message->num = atoi(tmp);
	tmp = strtok(NULL, " ");
	strcpy(message->string1, tmp);
	tmp = strtok(NULL, " ");
	strcpy(message->string2, tmp);
	return;
}

void CPlayer::check()
{
	int is_ok;
	char buffer[1];
	if (!connected) return;
	/*MSG_PEEK��ʾ����������ж����ݵ����������ݴ�����������Ƴ�*/
	recv(conn, buffer, 1, MSG_PEEK);
	is_ok = (WSAECONNRESET != WSAGetLastError());
	if (!is_ok)
	{
		strcpy(name, "Tourist");
		Num_Competition = 0;
		GPA = 0;
		state = LOBBY;
		connected = false;
	}
}
int comp(const void* p1, const void* p2)
{
	Player_Information i1 = *(Player_Information*)p1;
	Player_Information i2 = *(Player_Information*)p2;
	if (i1.GPA > i2.GPA) return -1;
	else if (i1.GPA < i2.GPA) return 1;
	else return 0;
}
