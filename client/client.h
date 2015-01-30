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
	long int current;				//������ ������� ������
	long int request_number;
	short int port_f,port_s;
	QString IP_str;

private:
	Ui::clientClass ui;
	void showCoin(int);						//���������� �����
	void update();							//��������� ������� �������
	void hide(bool controlle);				//���������� ��� ������������ ������� � ����������� �� ��������� �����
	void closeEvent(QCloseEvent * event);	//������� �������� ����
	//void openEvent();						//������� �������� ����
	Coin toCoin();
	Coin toCoin_();
	QString toStr(Coin);
	int Str_toChar(QString,char*);

private slots:          //�����:
	void saveName();	//���������� �������� ������ ��� ������ ��������������� �������� ������
	void saveNom();		//���������� �������� ��� ������ ��������������� �������� ������
	void saveEra (QListWidgetItem *);	//���������� ��� ��� ��������� �������� �������� ��������������� �������
	void saveYear();	//���������� ���� ������� ��� ��������� �������� ��������������� �������
	void saveCent();	//���������� ���� ��� ��������� �������� ��������������� �������
	void saveRari(bool);	//���������� �������� ��� ��������� ��������� ������ ���������������� �������
	void saveSPC (QListWidgetItem *);	//���������� ���-�� ������������� ��� ��������� �������� �������� �������
	void saveCOSC();	//���������� ���-�� ���������� ����� ��� ��������� �������� �������
	void saveFeat();		  //���������� �������� ��� ������ ��������������� �������� ������
	void deleteCoin();		  //�������� ������
	void fillCoin();	      //��������� 10 ��������
	void addCoin();			  //���������� ������
	void updateClient();
	void savePortf();
	void savePorts();
	void saveIP();
	void openEvent();
	void click(QTableWidgetItem *);
	void deleteEnable();	  //������������ ������ �������� ������
	void changedYeCe (int);    //���� ������� ���� ���("AD"), �� ������������ ���("CenturyLine")
	void changedYeCe1(int);   //���� ������� �� ����� ���("BC"), �� ������������ ���("YearLine")
	void changedCof  (int);	  //���� ���� �������("RarityF") �������, �� �����������-�������������("COSCline") ��������
	void changedCof1 (int);	  //���� ���� �������("RarityF") ���������, �� �����������-�������������("COSCline") ����������
};

#endif // CLIENT_H
