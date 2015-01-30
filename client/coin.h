/* Steshenko A.S. IVT-265 */

#pragma once

#include <QString> 

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
};
