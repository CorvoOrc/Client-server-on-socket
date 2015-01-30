/* Steshenko A.S. IVT-265 */

#include "coin.h"
#include <QString> 


Coin::Coin()//�����������
{
	//��������� �������:
	NameOfCoin = "default";		//��� ������ �� ���������
	Nominal = " ";				//������� �� �����
	Era[0]=1;Era[1]=0;			//�� ��������� ���������� ���� ���
	Year = 1;					//��� �������-1
	Century = 1;				//���-1
	isRarity = false;			//�� �������� ���������
	COSC = 1;					//1 ����(�/�)(������������� ���������� �����)
	SPC[0]=1;SPC[1]=0;SPC[2]=0; //�� ��������� �������� The Russian Empire
	Feature = " ";				//�� �������� ��������
	Id = 0;
}	

/*������� �������������� ����������� ������ Coin � Coin &other:
*\param  [in]  Coin &other - ������ �����
*\return-flag - ����, ������������, ����� �� ������
*/
bool Coin::operator==(const Coin other) const
{	
	bool flag=false;
	int index,index_other;
	for(int i=0;i<2;++i)
	{
		if(Era[i]==1)
			index=i;
		if(other.Era[i]==1)
			index_other=i;
	}
	if(isRarity == other.isRarity && index==index_other/*Year==other.Year && Century == other.Century*/ && NameOfCoin==other.NameOfCoin)
	{
		if(index==0)
			if(Year==other.Year)
				flag=true;
		else if(index==1)
			if(Century == other.Century)
				flag=true;
	}
	return flag;
}

/*�������, ����������� �� ����������� ������ Coin � Coin &other:
*\param  [in]  Coin &other - ������ �����
*\return-flag - ����, ������������, ������� �� ������
*/
bool Coin::operator!=(const Coin other) const
{	
	bool flag=true;
	int index,index_other;
	for(int i=0;i<2;++i)
	{
		if(Era[i]==1)
			index=i;
		if(other.Era[i]==1)
			index_other=i;
	}
	if(isRarity == other.isRarity &&  index==index_other && NameOfCoin==other.NameOfCoin)
	{
		if(index==0)
			if(Year==other.Year)
				flag=false;
		else if(index==1)
			if(Century == other.Century)
				flag=false;
	}
	return flag;
}

/*������� "������ ������"
*\param  [in]  Coin &other - ������ �����
*\return-flag - ����, ������������,������ �� ����� Coin ��� Coin &other
*/
bool Coin::operator>(const Coin other) const
{	
	bool flag=false;
	int index,index_other;
	for(int i=0;i<2;++i)
	{
		if(Era[i]==1)
			index=i;
		if(other.Era[i]==1)
			index_other=i;
	}
	if(isRarity == true && other.isRarity==false)
		flag=true;
	if(isRarity==other.isRarity && index>index_other)
		flag=true;
	if(isRarity==other.isRarity && index==index_other)
	{
		if(index==0)
		{
			if(Year<other.Year)
				flag=true;
			else if(Year==other.Year && NameOfCoin<other.NameOfCoin)
				flag=true;
		}
		else if(index==1)
		{
			if(Century<other.Century)
				flag=true;
			else if(Century==other.Century && NameOfCoin<other.NameOfCoin)
				flag=true;
		}
	}
	return flag;
}

/*������� "������ ������"
*\param  [in]  Coin &other - ������ �����
*\return-flag - ����, ������������,������ �� ����� Coin ��� Coin &other
*/
bool Coin::operator<(const Coin other) const
{	
	bool flag=false;
	int index,index_other;
	for(int i=0;i<2;++i)
	{
		if(Era[i]==1)
			index=i;
		if(other.Era[i]==1)
			index_other=i;
	}
	if(isRarity == false && other.isRarity==true)
		flag=true;
	if(isRarity == other.isRarity && index<index_other)
		flag=true;
	if(isRarity==other.isRarity && index==index_other)
	{
		if(index==0)
		{
			if(Year>other.Year)
				flag=true;
			else if(Year==other.Year && NameOfCoin>other.NameOfCoin)
				flag=true;
		}
		else if(index==1)
		{
			if(Century>other.Century)
				flag=true;
			else if(Century==other.Century && NameOfCoin>other.NameOfCoin)
				flag=true;
		}
	}
	return flag;
}

/*������� "�������� ������"
*\param  [in]  Coin &other - ������ �����
*\return 0  - ����  ����� Coin ������������ ������ Coin &other
*\return 1  - ����  ����� Coin ������ ������ Coin &other
*\return -1 - ����  ����� Coin ������ ������ Coin &other
*/
int Coin::operator>=(const Coin other) const
{
	if(isRarity == true && other.isRarity==false)
	{
		return 1;
	}
	else if(isRarity == other.isRarity)
	{
		if(Year<other.Year || Century<other.Century)
		{
			return 1;
		}
	}
	else if(isRarity == other.isRarity)
	{
		if(Year == other.Year && Century == other.Century)
		{
			if(NameOfCoin<other.NameOfCoin)
			{
				return 1;
			}
		}
	}
	else if(isRarity == other.isRarity)
	{
		if(Year == other.Year && Century == other.Century)
		{
			if(NameOfCoin == other.NameOfCoin)
			{
				return 0;
			}
		}
	}
	return -1;
}

/*������� "�������� ������"
*\param  [in]  Coin &other - ������ �����
*\return 0  - ����  ����� Coin ������������ ������ Coin &other
*\return 1  - ����  ����� Coin ������ ������ Coin &other
*\return -1 - ����  ����� Coin ������ ������ Coin &other
*/
int Coin::operator<=(const Coin other) const
{	
	if(isRarity == false && other.isRarity==true)
	{
		return 1;
	}
	else if(isRarity == other.isRarity)
	{
		if(Year>other.Year || Century>other.Century)
		{
			return 1;
		}
	}
	else if(isRarity == other.isRarity)
	{
		if(Year == other.Year && Century == other.Century)
		{
			if(NameOfCoin>other.NameOfCoin)
			{
				return 1;
			}
		}
	}
	else if(isRarity == other.isRarity)
	{
		if(Year == other.Year && Century == other.Century)
		{
			if(NameOfCoin == other.NameOfCoin)
			{
				return 0;
			}
		}
	}
	return -1;
}

void Coin::operator=(const Coin other)
{	
	NameOfCoin=other.NameOfCoin;
	Nominal=other.Nominal;
	Era[0]=other.Era[0];	
	Era[1]=other.Era[1];
	Year=other.Year;
	YearInStr=other.YearInStr;
	Century=other.Century;
	CenturyInString=other.CenturyInString;
	isRarity=other.isRarity;
	COSC=other.COSC;
	COSCinString=other.COSCinString;
	SPC[0]=other.SPC[0];
	SPC[1]=other.SPC[1];
	SPC[2]=other.SPC[2];
	Feature=other.Feature;
	Id = other.Id;
}