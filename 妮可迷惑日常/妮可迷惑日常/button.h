#pragma once
#include<Windows.h>
class Button
{
public:
	RECT boarder[3]; //����Ϊ���δ�ƹ�������ƹ�����갴��ʱ�İ�ť�߽�
	void Init();
	char* Style[3];//��״̬ʱ��ͼ���ַ
	int m_ID;
};

