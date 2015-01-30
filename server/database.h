/* Steshenko A.S. IVT-265 */

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include "coin.h"
#include <QList>
#include <QListIterator>

class DataBase
{
public:
	DataBase();
	int  count();
	int  searchId(int Id_);
	int  sort(int Id_);
	void clear();
	int append(Coin other);
	void remove(int i);
	void save(int i, int field, QString shift);
	void save(int i, int field, int shift);
	QString  getId(int i);
	QString record(int i);
	QString record_str(int i);
	QList <Coin> records();
private:
	QList <Coin> base;
	int maxId;
};

#endif // DATABASE_H
