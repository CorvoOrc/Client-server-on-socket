/* Steshenko A.S. IVT-265 */

#include "database.h"
#include "coin.h"
#include <QList>
#include <QListIterator>

DataBase::DataBase(){
	maxId = 0;
}

int DataBase::count(){
	return base.count();
}

void DataBase::clear(){
	base.clear();
}

int DataBase::append(Coin other){
	other.Id = ++maxId;
	base.append(other);
	return maxId;
}

void DataBase::remove(int i){
	base.removeAt(i);
}

int DataBase::searchId(int Id_){
	int i;
	int count = base.count();
	for(i=0;i<count;++i)
		if(Id_ == base[i].Id)
			return i;
	return -1;
}
QString DataBase::getId(int i){
	QString StrId = StrId.number(base[i].Id);
	return StrId;
}

QString DataBase::record(int i){
	return base.value(i).show();
}

QString DataBase::record_str(int i){
	QString str = ("");
	QString tmp = ("");
	QString one =		("1");
	QString sep_record=	(";");
	QString sep_records=("|");
	QString zero=		("0");
	str.append(base[i].NameOfCoin);
	str.append(sep_record);
	str.append(base[i].Nominal);
	str.append(sep_record);
	str.append(tmp.number(base[i].Era[0]));
	str.append(sep_record);
	str.append(tmp.number(base[i].Era[1]));
	str.append(sep_record);
	str.append(tmp.number(base[i].Year));
	str.append(sep_record);
	str.append(tmp.number(base[i].Century));
	str.append(sep_record);
	if(base[i].isRarity)
		str.append( one);
	else
		str.append( zero );
	str.append(sep_record);
	str.append(tmp.number(base[i].COSC));
	str.append(sep_record);
	str.append(tmp.number(base[i].SPC[0]));
	str.append(sep_record);
	str.append(tmp.number(base[i].SPC[1]));
	str.append(sep_record);
	str.append(tmp.number(base[i].SPC[2]));
	str.append(sep_record);
	str.append(base[i].Feature);
	return str;
}

QList <Coin> DataBase::records(){
	return base;
}

void DataBase::save(int i, int field, QString shift){
	base[i].edit_s(field,shift);
}

void DataBase::save(int i, int field, int shift){
	base[i].edit_i(field,shift);
}

int DataBase::sort(int Id_){
	int i,j;
	int count = base.count();
	for(i=0;i<count-1;++i)
		for(j=i+1;j<count;++j)
			if(base[j]>base[i])
				qSwap(base[i], base[j]);//меняем записи

	for(i=0;i<count;++i)//for(i=0;i<base.count()-1;++i)
		if(Id_==base[i].Id)
			return i;
	return -1;
}