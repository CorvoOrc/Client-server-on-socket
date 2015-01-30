/* Steshenko A.S. IVT-265 */

#ifndef CLIENT_H
#define CLIENT_H

#include <QtGui/QMainWindow>
#include <WinSock2.h>
#include "ui_client.h"
#include <QTimer>
#include <QString>
#include <QList>
#include <QListIterator>
#include <QMessageBox>
#include <QtGui>
#include "nominalvalid.h"
#include "coin.h"
#include <windows.h>

class client : public QMainWindow
{
	Q_OBJECT

public:
	client(QWidget *parent = 0, Qt::WFlags flags = 0);
	~client();
	SOCKET sock_new,sock_upd;
	bool remove, addition, filling;
	long int count;
	long int current;				//индекс текущей записи
	long int request_number;
	short int port_f,port_s;
	QString IP_str;

private:
	Ui::clientClass ui;
	void showCoin(int);						//показываем форму
	void update();							//обновляет браузер записей
	void hide(bool controlle);				//активирует или деактивирует виждеты в зависимости от состояния флага
	void closeEvent(QCloseEvent * event);	//событие закрытия окна
	//void openEvent();						//событие открытия окна
	Coin toCoin();
	Coin toCoin_();
	QString toStr(Coin);
	int Str_toChar(QString,char*);

private slots:          //СЛОТЫ:
	void saveName();	//сохранение названия монеты при потере соответствующим виджетом фокуса
	void saveNom();		//сохранение номинала при потере соответствующим виджетом фокуса
	void saveEra (QListWidgetItem *);	//сохранение эры при изменении текущего элемента соответствующим виджета
	void saveYear();	//сохранение года выпуска при изменении значения соответствующим виджета
	void saveCent();	//сохранение века при изменении значения соответствующим виджета
	void saveRari(bool);	//сохранение раритета при изменении состояния флажка соответствующего виджета
	void saveSPC (QListWidgetItem *);	//сохранение гос-ва производителя при изменение текущего элемента виджета
	void saveCOSC();	//сохранение кол-ва раритетных монет при изменении значения виджета
	void saveFeat();		  //сохранение описания при потере соответствующим виджетом фокуса
	void deleteCoin();		  //удаление монеты
	void fillCoin();	      //заполнить 10 записями
	void addCoin();			  //добавление монеты
	void updateClient();
	void savePortf();
	void savePorts();
	void saveIP();
	void openEvent();
	void click(QTableWidgetItem *);
	void deleteEnable();	  //деактивируем кнопку удаления монеты
	void changedYeCe (int);    //если выбрана наша эра("AD"), то деактивируем век("CenturyLine")
	void changedYeCe1(int);   //если выбрано до нашей эры("BC"), то деактивируем год("YearLine")
	void changedCof  (int);	  //если флаг раритет("RarityF") активен, то государство-производитель("COSCline") активено
	void changedCof1 (int);	  //если флаг раритет("RarityF") неактивен, то государство-производитель("COSCline") неактивено
};

#endif // CLIENT_H
