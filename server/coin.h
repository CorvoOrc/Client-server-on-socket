/* Steshenko A.S. IVT-265 */
#pragma once

#include <QString> 
#include <windows.h>
#include <iostream>

using namespace std;

// �����, ����������� ������
class Coin
{
public:	//����� ������
	Coin();//�����������
	// �������� "�����" ���� ���������
	bool operator==(const Coin other) const;
	// �������� "�������" ���� ���������
	bool operator!=(const Coin other) const;
	// �������� "������ ������" ���� �����
	bool operator>(const Coin other) const;
	// �������� "������ ������" ���� ����
	bool operator<(const Coin other) const;
	// �������� "�� ������ ������" ���� ����
	int operator>=(const Coin other) const;
	// �������� "�� ������ ������" ���� ����
	int operator<=(const Coin other) const;
	// �������� �����������
	void operator=(const Coin other);
	// �������� ���������
	void edit_s(int field,QString shift);
	void edit_i(int field,int	  shift);
	QString show();

	QString NameOfCoin;	    //�������� ������
	QString Nominal;		//�������
	int Era[2];				//������������ ���
	int Year;				//��� �������
	QString YearInStr;	    //�������� ���������� ��� �������������� ���� Year(���� �������)
	int Century;		    //���
	QString CenturyInString;//�������� ���������� ��� �������������� ���� Century(����)
	bool isRarity;		    //�������� �� ������ ���������
	int COSC;			    //���������� ���������� ����������� �����
	QString COSCinString;   //�������� ���������� ��� �������������� ���� COSC(������ �������������)
	int SPC[3];			    //������������ ����������, ��� ������ ���� �����������
	QString Feature;	    //�������� ������
	int Id;
	HANDLE hLocke,hLockr;
};
