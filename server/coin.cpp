#include "coin.h"
#include "database.h"
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

	hLocke = CreateEvent(NULL,TRUE,true,(LPCWSTR)"locke");
	hLockr = CreateEvent(NULL,TRUE,true,(LPCWSTR)"lockr");
}	

void Coin::edit_s(int field,QString shift){
	HANDLE LockGroup[2];
	LockGroup[0] = hLocke;
	LockGroup[1] = hLockr;
	WaitForMultipleObjects(2,LockGroup,1,INFINITE);
	//cout<<"Lock_e"<<endl;
	ResetEvent(hLocke);
	int name	=0;
	int nominal	=1;
	int rarity	=2;
	int feature	=3;
	QString one	="1";

	if(field==name)
		NameOfCoin=shift;
	else if(field==nominal)
		Nominal=shift;
	else if(field==feature)
		Feature=shift;
	else if(field==rarity){
		if(shift==one)
			isRarity=true;
		else
			isRarity=false;
	}
	SetEvent(hLocke);
	//cout<<"Lock_e exit"<<endl;
}

void Coin::edit_i(int field,int shift){
	HANDLE LockGroup[2];
	LockGroup[0] = hLocke;
	LockGroup[1] = hLockr;
	WaitForMultipleObjects(2,LockGroup,1,INFINITE);
	ResetEvent(hLocke);
	//Sleep(10000);
	//cout<<"Lock_e"<<endl;
	int era		=0;
	int year	=1;
	int century	=2;
	int cosc	=3;
	int spc		=4;

	if(field==year)
		Year=shift;
	else if(field==century)
		Century=shift;
	else if(field==cosc)
		COSC=shift;
	else if(field==era){
		if(shift==0){
			Era[0]=1;
			Era[1]=0;
		}
		else if(shift==1){
			Era[0]=0;
			Era[1]=1;
		}
	}
	else if(field==spc){
		if(shift==0){
			SPC[0]=1;
			SPC[1]=0;
			SPC[2]=0;
		}
		else if(shift==1){
			SPC[0]=0;
			SPC[1]=1;
			SPC[2]=0;
		}
		else if(shift==2){
			SPC[0]=0;
			SPC[1]=0;
			SPC[2]=1;
		}
	}
	SetEvent(hLocke);
	//cout<<"Lock_e exit"<<endl;
}

QString Coin::show(){
	WaitForSingleObject(hLocke,INFINITE);
	ResetEvent(hLockr);
	//cout<<"Lock_r"<<endl;
	QString str = ("");
	QString tmp = ("");
	QString one =		("1");
	QString sep_record=	(";");
	QString sep_records=("|");
	QString zero=		("0");
	str.append(NameOfCoin);
	str.append(sep_record);
	str.append(Nominal);
	str.append(sep_record);
	str.append(tmp.number(Era[0]));
	str.append(sep_record);
	str.append(tmp.number(Era[1]));
	str.append(sep_record);
	str.append(tmp.number(Year));
	str.append(sep_record);
	str.append(tmp.number(Century));
	str.append(sep_record);
	if(isRarity)
		str.append( one);
	else
		str.append( zero );
	str.append(sep_record);
	str.append(tmp.number(COSC));
	str.append(sep_record);
	str.append(tmp.number(SPC[0]));
	str.append(sep_record);
	str.append(tmp.number(SPC[1]));
	str.append(sep_record);
	str.append(tmp.number(SPC[2]));
	str.append(sep_record);
	str.append(Feature);
	SetEvent(hLockr);
	//cout<<"Lock_r exit"<<endl;
	return str;
}

/*Функция эквивалентного сравнивания класса Coin с Coin &other:
*\param  [in]  Coin &other - другой класс
*\return-flag - флаг, показывающий, равны ли классы
*/
bool Coin::operator==(const Coin other) const
{	
	bool flag=false;
	int index,index_other;
	for(int i=0;i<2;++i){
		if(Era[i]==1)
			index=i;
		if(other.Era[i]==1)
			index_other=i;
	}
	if(isRarity == other.isRarity && index==index_other && NameOfCoin==other.NameOfCoin){
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
	for(int i=0;i<2;++i){
		if(Era[i]==1)
			index=i;
		if(other.Era[i]==1)
			index_other=i;
	}
	if(isRarity == other.isRarity &&  index==index_other && NameOfCoin==other.NameOfCoin){
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
	for(int i=0;i<2;++i){
		if(Era[i]==1)
			index=i;
		if(other.Era[i]==1)
			index_other=i;
	}
	if(isRarity == true && other.isRarity==false)
		flag=true;
	if(isRarity==other.isRarity && index>index_other)
		flag=true;
	if(isRarity==other.isRarity && index==index_other){
		if(index==0){
			if(Year<other.Year)
				flag=true;
			else if(Year==other.Year && NameOfCoin<other.NameOfCoin)
				flag=true;
		}
		else if(index==1){
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
	for(int i=0;i<2;++i){
		if(Era[i]==1)
			index=i;
		if(other.Era[i]==1)
			index_other=i;
	}
	if(isRarity == false && other.isRarity==true)
		flag=true;
	if(isRarity == other.isRarity && index<index_other)
		flag=true;
	if(isRarity==other.isRarity && index==index_other){
		if(index==0){
			if(Year>other.Year)
				flag=true;
			else if(Year==other.Year && NameOfCoin>other.NameOfCoin)
				flag=true;
		}
		else if(index==1){
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
	if(isRarity == true && other.isRarity==false){
		return 1;
	}
	else if(isRarity == other.isRarity){
		if(Year<other.Year || Century<other.Century){
			return 1;
		}
	}
	else if(isRarity == other.isRarity){
		if(Year == other.Year && Century == other.Century){
			if(NameOfCoin<other.NameOfCoin){
				return 1;
			}
		}
	}
	else if(isRarity == other.isRarity){
		if(Year == other.Year && Century == other.Century){
			if(NameOfCoin == other.NameOfCoin){
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
	if(isRarity == false && other.isRarity==true){
		return 1;
	}
	else if(isRarity == other.isRarity){
		if(Year>other.Year || Century>other.Century){
			return 1;
		}
	}
	else if(isRarity == other.isRarity){
		if(Year == other.Year && Century == other.Century){
			if(NameOfCoin>other.NameOfCoin){
				return 1;
			}
		}
	}
	else if(isRarity == other.isRarity){
		if(Year == other.Year && Century == other.Century){
			if(NameOfCoin == other.NameOfCoin){
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