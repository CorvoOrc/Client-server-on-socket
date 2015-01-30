/* Steshenko A.S. IVT-265 */

#include "coin.h"
#include <QString> 


Coin::Coin()//конструктор
{
	//начальные условия:
	NameOfCoin = "default";		//имя монеты по умолчанию
	Nominal = " ";				//номинал не задан
	Era[0]=1;Era[1]=0;			//по умолчанию выбирается наша эра
	Year = 1;					//год выпуска-1
	Century = 1;				//век-1
	isRarity = false;			//не является раритетом
	COSC = 1;					//1 штук(и/а)(сохранившихся раритетных монет)
	SPC[0]=1;SPC[1]=0;SPC[2]=0; //по умолчанию выбираем The Russian Empire
	Feature = " ";				//не заданное описание
	Id = 0;
}	

/*Функция эквивалентного сравнивания класса Coin с Coin &other:
*\param  [in]  Coin &other - другой класс
*\return-flag - флаг, показывающий, равны ли классы
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

/*Функция, проверяющая на неравенство классы Coin и Coin &other:
*\param  [in]  Coin &other - другой класс
*\return-flag - флаг, показывающий, неравны ли классы
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

/*Функция "строго больше"
*\param  [in]  Coin &other - другой класс
*\return-flag - флаг, показывающий,больше ли класс Coin чем Coin &other
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

/*Функция "строго меньше"
*\param  [in]  Coin &other - другой класс
*\return-flag - флаг, показывающий,меньше ли класс Coin чем Coin &other
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

/*Функция "нестрого больше"
*\param  [in]  Coin &other - другой класс
*\return 0  - если  класс Coin еквивалентен классу Coin &other
*\return 1  - если  класс Coin больше класса Coin &other
*\return -1 - если  класс Coin меньше класса Coin &other
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

/*Функция "нестрого меньше"
*\param  [in]  Coin &other - другой класс
*\return 0  - если  класс Coin еквивалентен классу Coin &other
*\return 1  - если  класс Coin меньше класса Coin &other
*\return -1 - если  класс Coin больше класса Coin &other
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