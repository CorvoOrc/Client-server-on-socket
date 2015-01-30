/* Steshenko A.S. IVT-265 */

#pragma once

#include <QString> 

// Класс, описывающий монету
class Coin
{
public:	//общий доступ
	Coin();//конструктор
	// Операция "равно" двух раритетов
	bool operator==(const Coin other) const;
	// Операция "неравно" двух раритетов
	bool operator!=(const Coin other) const;
	// Операция "строго больше" двух годов
	bool operator>(const Coin other) const;
	// Операция "строго меньше" двух имен
	bool operator<(const Coin other) const;
	// Операция "не строго больше" двух имен
	int operator>=(const Coin other) const;
	// Операция "не строго меньше" двух имен
	int operator<=(const Coin other) const;
	// Операция присвивания
	void operator=(const Coin other);

	QString NameOfCoin;	    //название монеты
	QString Nominal;		//номинал
	int Era[2];				//перечисление эры
	int Year;				//год выпуска
	QString YearInStr;	    //буферная переменная для преобразования типа Year(года выпуска)
	int Century;		    //век
	QString CenturyInString;//буферная переменная для преобразования типа Century(века)
	bool isRarity;		    //является ли монета раритетом
	int COSC;			    //количество раритетных сохраненных монет
	QString COSCinString;   //буферная переменная для преобразования типа COSC(страна производитель)
	int SPC[3];			    //перечисление государств, где монета была произведена
	QString Feature;	    //описание монеты
	int Id;
};
