#pragma once
#include<Windows.h>
class CButton
{
public:
	void Init();

private:
	RECT boarder[3]; //����Ϊ���δ�ƹ�������ƹ�����갴��ʱ�İ�ť�߽�
	char* Style[3];//��״̬ʱ��ͼ���ַ
	int m_ID;
};

