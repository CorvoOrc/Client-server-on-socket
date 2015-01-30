/* Steshenko A.S. IVT-265 */

#include "client.h"
#include <WinSock2.h>
#include "coin.h"
#include <QTimer>
#include <QString>
#include <QList>
#include <QListIterator>
#include <QMessageBox>
#include <QtGui>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <ATLCONV.H>

client::client(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	//������������� ���������������� ���������
	ui.setupUi(this);

	//������������ �������
	hide(false);

	//���������� ��������� �� Nominal(�������)
	/*QValidator * exValid=new NominalValid(this);
	ui.NominalLine->setValidator(exValid);*/

	//������������� ����������� �������� �� ����������� ������ in Era
	ui.EraList->insertItem(0,QString("AD"));
	ui.EraList->insertItem(1,QString("BC"));

	//������������� ����������� �������� �� ����������� ������ in SPC
	ui.SPClist->insertItem(0,QString("The Russian Empire"));
	ui.SPClist->insertItem(1,QString("USSR"));
	ui.SPClist->insertItem(2,QString("Russia"));
	
	//���������� ������ � ����� ����������� ��������
	QTimer *timer=new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateClient()));
    timer->start(10000);
	connect(ui.coinsTable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(click(QTableWidgetItem*)));
	connect(ui.addB,SIGNAL	 (clicked()),this,SLOT(addCoin()));
	connect(ui.goB,SIGNAL	 (clicked()),this,SLOT(openEvent()));
	connect(ui.deleteB,SIGNAL(clicked()),this,SLOT(deleteCoin()));
	connect(ui.fillB,SIGNAL  (clicked()),this,SLOT(fillCoin()));
	connect(ui.deleteB,SIGNAL(clicked()),this,SLOT(deleteEnable()));

	connect(ui.portfLine, SIGNAL(editingFinished ()), this, SLOT(savePortf()));
	connect(ui.portsLine, SIGNAL(editingFinished ()), this, SLOT(savePorts()));
	connect(ui.IPLine,	  SIGNAL(editingFinished ()), this, SLOT(saveIP()));

	//���������� ������� ��� ��������� ��������� ������� � ����� ���������� ������
	connect(ui.NameOfCoinLine, SIGNAL(editingFinished ()), this, SLOT(saveName()));
	connect(ui.EraList, SIGNAL (itemClicked(QListWidgetItem *)), this, SLOT(saveEra(QListWidgetItem *)));
	connect(ui.NominalLine, SIGNAL(editingFinished()), this, SLOT(saveNom()));
	connect(ui.YearLine, SIGNAL(editingFinished()), this, SLOT(saveYear()));
	connect(ui.CenturyLine, SIGNAL(editingFinished()), this, SLOT(saveCent()));
	connect(ui.RarityF, SIGNAL(clicked ( bool)), this, SLOT(saveRari(bool)));
	connect(ui.SPClist, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(saveSPC(QListWidgetItem *)));
	connect(ui.COSCline, SIGNAL(editingFinished()), this, SLOT(saveCOSC()));
	connect(ui.FeatureLine, SIGNAL(editingFinished ()), this, SLOT(saveFeat()));

	//��� ������ ���� ���� ��� ����
	connect(ui.EraList, SIGNAL(currentRowChanged(int)),this, SLOT(changedYeCe(int)));
	connect(ui.EraList, SIGNAL(currentRowChanged(int)),this, SLOT(changedYeCe1(int)));

	//��� ���������/����������� ���� ���������� ������������� �����
	connect(ui.RarityF, SIGNAL(stateChanged(int)),this, SLOT(changedCof(int)));
	connect(ui.RarityF, SIGNAL(stateChanged(int)),this, SLOT(changedCof1(int)));

	current=0;//������� ������ �������
	count=0;
	remove = false, addition = false, filling = false;

	ui.CenturyLine	->setEnabled(false);//������������ ���� Century(����)
	ui.deleteB		->setEnabled(false);	  //������������ ���� �������� ������
	ui.goB			->setEnabled(false);

	ui.fillB	->hide();
	ui.addB		->hide();
	ui.deleteB	->hide();


	//openEvent();//������ �� ���� ������
}

client::~client()
{

}

//����� ������ ��� ������
const int size_str=1000;

/*!������� ���������� ���� ���� ������� � ����������� �� ���
*\param  [in]  t - �������� ��� ������������ � ��������
*/
void client::changedYeCe(int t)
{
	if (ui.EraList->currentRow() == 0)
	{
		ui.YearLine		->setEnabled(true);
		ui.CenturyLine	->setEnabled(false);
	}
}

/*!������� ���������� ���� ���� � ����������� �� ���
*\param  [in]  t - �������� ��� ������������ � ��������
*/
void client::changedYeCe1(int t)
{
	if(ui.EraList->currentRow() == 1)
	{
		ui.YearLine->	setEnabled(false);
		ui.CenturyLine->setEnabled(true);
	}
}

/*!������� ��������� ���� COSC, ���� ������� true
*\param  [in]  t - �������� ��� ������������ � ��������
*/
void client::changedCof(int t)
{
	
	if (ui.RarityF->isChecked())
	{
		ui.COSCline->setEnabled(true);
	}
}

/*!������� ����������� ���� COSC, ���� ������� false
*\param  [in]  t - �������� ��� ������������ � ��������
*/
void client::changedCof1(int t)
{
	if(!ui.RarityF->isChecked())
	{
		ui.COSCline->setEnabled(false);
	}
}

/*!������� ����������� ������ �������� ������
*/
void client::deleteEnable()
{
	if(count==0)
	{
		ui.deleteB->setEnabled(false);
	}
}

/*!������� ���������� ��� ������������ ������� � ����������� �� ��������� �����
*\param  [in] controlle - ��������� �����
*/
void client::hide(bool controlle)
{
	ui.NameOfCoinLine->	setEnabled(controlle);
	ui.NominalLine->	setEnabled(controlle);
	ui.EraList->		setEnabled(controlle);
	ui.YearLine->		setEnabled(controlle);
	ui.CenturyLine->	setEnabled(controlle);
	ui.RarityF->		setEnabled(controlle);
	ui.COSCline->		setEnabled(controlle);
	ui.SPClist->		setEnabled(controlle);
	ui.FeatureLine->	setEnabled(controlle);
}

int client::Str_toChar(QString str,char* c_str)
{
	QByteArray ba=str.toAscii();
	char * rec=ba.data();
	const int size=str.size();
	for(int i=0;i<size;++i)
		c_str[i]=rec[i];
	return size;
}

QString client::toStr(Coin other)
{
	QString str("");
	QString tmp("");
	QString sep_record=	(";");
	QString sep_records=("|");
	QString one=		("1");
	QString zero=		("0");

	str.append( other.NameOfCoin );
	str.append(sep_record);
	str.append( other.Nominal);
	str.append(sep_record);
	str.append( tmp.number( other.Era[0] ) );
	str.append(sep_record);
	str.append( tmp.number( other.Era[1] ) );
	str.append(sep_record);
	str.append( tmp.number( other.Year ) );
	str.append(sep_record);
	str.append( tmp.number( other.Century ) );
	str.append(sep_record);
	if(other.isRarity)
		str.append( one );
	else
		str.append( zero );
	str.append(sep_record);
	str.append( tmp.number( other.COSC ) );
	str.append(sep_record);
	str.append( tmp.number( other.SPC[0] ) );
	str.append(sep_record);
	str.append( tmp.number( other.SPC[1] ) );
	str.append(sep_record);
	str.append( tmp.number( other.SPC[2] ) );
	str.append(sep_record);
	str.append( other.Feature );
	str.append(sep_records);

	return str;
}

void client::savePortf(){
	port_f=ui.portfLine->text().toInt();
}
void client::savePorts(){
	port_s=ui.portsLine->text().toInt();
}
void client::saveIP(){
	IP_str = ui.IPLine->text();
	ui.goB	->setEnabled(true);
}
Coin client::toCoin()
{
	USES_CONVERSION;
	int i=0,j=0,buffer=0;
	Coin temp;
	DWORD bytesread;
	long int size;
	char c_str[1000]={NULL};
	//ReadFile(hPipe,(void*)&size,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&size, sizeof(long int),0);
	size = ntohl(size);
	//ReadFile(hPipe,(void*)&c_str,sizeof(char)*size,&bytesread,NULL);
	recv(sock_new,c_str, sizeof(char)*size,0);
	QString str=(c_str);
	QString one=("1");
	QString tmp=("");

	buffer=str.indexOf(";",j+1);
	temp.NameOfCoin=tmp.insert(0,str.mid(j,buffer-j));tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.Nominal=tmp.insert(0,str.mid(j+1,buffer-j));tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.Era[0]=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.Era[1]=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.Year=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.Century=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	tmp.insert(0,str.mid(j+1,buffer-j));
	if(tmp==one)
		temp.isRarity=true;
	else
		temp.isRarity=false;
	tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.COSC=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.SPC[0]=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.SPC[1]=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.SPC[2]=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();

	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.Feature=tmp.insert(0,str.mid(j+1,buffer-j));tmp.clear();
	j=str.indexOf(";",j+1);
	i=str.indexOf("|",i+1);
	if(i == -1)
		return temp;
	buffer=i-j-1;
	temp.Id=tmp.insert(0,str.mid(j+1,buffer)).toInt();tmp.clear();

	return temp;
}

/*!������� ���������� �������� �������
*/
void client::update()
{
	USES_CONVERSION;
	DWORD bytesread,byteswritten;
	request_number=10;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	long int request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	//ReadFile(hPipe,(void*)&count,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&count, sizeof(long int),0);
	count = ntohl(count);
	ui.coinsTable->setRowCount(count);			//������������� ���������� ������� � ��������
    QTableWidgetItem *item;						//������ � �������
	request_number=18;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);

    //������� ��� ������ � �������
    for (int i=0;i<count;++i)
    {
		Coin temp;
		temp=toCoin();

		item = new  QTableWidgetItem(temp.NameOfCoin);//������ ����� ������
		ui.coinsTable->setItem(i, 0, item);	//� ������ �������

		//���� ���� ���,
		if(temp.Era[0]==1)
		{
			//�� ������ ���� ������� � ����������� �� ���������
			if(temp.Year>=0 && temp.Year<10)
			{
				item = new QTableWidgetItem(temp.YearInStr.number(temp.Year).insert(0,"000"));
			}
			else if(temp.Year>9 && temp.Year<100)
			{
				item = new QTableWidgetItem(temp.YearInStr.number(temp.Year).insert(0,"00"));
			}
			else if(temp.Year>99 && temp.Year<1000)
			{
				item = new QTableWidgetItem(temp.YearInStr.number(temp.Year).insert(0,"0"));
			}
			else if(temp.Year>999 && temp.Year<2012)
			{
				item = new QTableWidgetItem(temp.YearInStr.number(temp.Year));
			}
			ui.coinsTable->setItem(i, 1, item);//�� ������ �������
		}
		//���� �� ����� ���
		else if(temp.Era[1]==1)
		{
			//������ ���� � ����������� �� ���������
			if(temp.Century>=1 && temp.Century<10)
			{
				item = new QTableWidgetItem(temp.CenturyInString.number(temp.Century).insert(2," BC"));
			}
			else if(temp.Century>9 && temp.Century<=21)
			{
				item = new QTableWidgetItem(temp.CenturyInString.number(temp.Century).insert(2," BC"));
			}
			ui.coinsTable->setItem(i, 1, item);//�� ������ �������
		}

		//���� ������ ����������
		if(temp.isRarity)
		{
			item = new QTableWidgetItem(temp.COSCinString.number(temp.COSC));//������ COSC
			ui.coinsTable->setItem(i ,2, item);//� ������ �������
		}
		//����� ���� ������ �� ����������
		else if(!temp.isRarity)
		{
			item = new QTableWidgetItem("no rarities");//������ ������ ����������� "no rarities"
			ui.coinsTable->setItem(i ,2, item);
		}
		QString IdStr = IdStr.number(temp.Id);
		item = new QTableWidgetItem(IdStr);
		ui.coinsTable->setItem(i ,3, item);
    }
}

void client::click(QTableWidgetItem * current)
{
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	showCoin(current->row());
}

/*!������� ���������� ����� ������(������ �� ��������)*/
void client::showCoin(int i)
{
	USES_CONVERSION;
	QTableWidgetItem * cur_rec;
	cur_rec = ui.coinsTable->item(i,3);
	int Id_ = cur_rec->text().toInt();
	long int Pos=0;

	request_number=16;
	//������ � ���������� ������
	DWORD byteswritten,bytesread;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	long int request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
	long int Id_right = (long int) Id_;
	Id_right=htonl(Id_right);
	send(sock_new,(char *)&Id_right,sizeof(long int),0);
	//ReadFile(hPipe,	 (void*)&Pos,			 sizeof(int), &bytesread,NULL);
	recv(sock_new,(char *)&Pos, sizeof(long),0);
	Pos = ntohl(Pos);
	if(Pos == -1){
		ui.NameOfCoinLine->setText("Record no exist!");
		hide(false);
		return ;
	}

	Coin temp;
	temp=toCoin();

	hide(true);
	//������������� �������� ��������
	ui.NameOfCoinLine->setText(temp.NameOfCoin);
	ui.NominalLine->setText(temp.Nominal);
	int indexEra;
	for(int i=0;i<2 ;++i)
		if(temp.Era[i]==1)
			indexEra=i;
	ui.EraList->setCurrentRow(indexEra);
	if(indexEra==0)
	{
		ui.CenturyLine->setEnabled(false);
		ui.YearLine->setEnabled(true);
	}
	else
	{
		ui.YearLine->setEnabled(false);
		ui.CenturyLine->setEnabled(true);
	}
	ui.YearLine->setValue(temp.Year);
	ui.CenturyLine->setValue(temp.Century);
	ui.RarityF->setChecked(temp.isRarity);
	if(temp.isRarity)
		ui.COSCline->setEnabled(true);
	else
		ui.COSCline->setEnabled(false);
	ui.COSCline->setValue(temp.COSC);
	int indexSPC;
	for(int i=0;i<3;++i)
		if(temp.SPC[i]==1)
			indexSPC=i;
	ui.SPClist->setCurrentRow(indexSPC);
	ui.FeatureLine->setText(temp.Feature);

	ui.coinsTable->selectRow(Pos);
}

/*!������� ��������� ������*/
void client::addCoin()
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	hide(true);			//���������� ������� ��������������
	DWORD bytesread,byteswritten;
	long int insertPos=0,Id_=0;
	Coin temp;			//��������� ������ ��� ������������ ��������������
	//������ � ���������� ������
	request_number=12;
	addition = true;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	long int request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	QString str = ("");
	str=toStr(temp);
	char c_str[2000]={NULL};
	int len;
	len=Str_toChar(str,c_str);
	//WriteFile(hPipe, (void*)&len, sizeof(int), &byteswritten, NULL);
	long int len_right = (long int) len;
	len_right=htonl(len_right);
	send(sock_new,(char *)&len_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&c_str, sizeof(char)*len, &byteswritten, NULL);
	send(sock_new,c_str,sizeof(char)*len,0);
	//ReadFile(hPipe,(void*)&Id_,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&Id_, sizeof(long),0);
	Id_ = ntohl(Id_);

	//������ � ����������
	request_number=17;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&Id_, sizeof(int), &byteswritten, NULL);
	long int Id_right = (long int) Id_;
	Id_right=htonl(Id_right);
	send(sock_new,(char *)&Id_right,sizeof(long int),0);
	//������ ������ ������ ����� ���������� �� ������
	//ReadFile(hPipe,(void*)&insertPos,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&insertPos, sizeof(long int),0);
	insertPos = ntohl(insertPos);

	update();
	showCoin(insertPos);
	ui.coinsTable->selectRow(insertPos);
	++count;

	//��� ��� ���������� ������, �� ����� �� �������,
	if (count >= 1){
		ui.deleteB->setEnabled(true);//���������� ������ �������� ������
	}
	addition = false;
}

/*!������� ������� ������*/
void client::deleteCoin()
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	DWORD bytesread,byteswritten;
	//������������� ������ �������� ��������
	current=ui.coinsTable->currentRow();
	if(current == -1)
		return ;
	request_number=10;
	//������ �� ���-�� �������
	//WriteFile(hPipe,(void*)&request_number,sizeof(int),&byteswritten,NULL);
	long int request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	//ReadFile(hPipe,(void*)&count,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&count, sizeof(long int),0);
	count = ntohl(count);

	remove = true;
	QTableWidgetItem * cur_rec;
	cur_rec = ui.coinsTable->item(current,3);
	int Id_ = cur_rec->text().toInt();
	request_number=13;
	//������ �� ��������
	//WriteFile(hPipe,(void*)&request_number,sizeof(int),&byteswritten,NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	//WriteFile(hPipe,(void*)&Id_,sizeof(int),&byteswritten,NULL);
	long int Id_right = (long int) Id_;
	Id_right=htonl(Id_right);
	send(sock_new,(char *)&Id_right,sizeof(long int),0);

	//���� ������� ������ �� ���������
	if( current!=count-1 && count!=0 && current!=0 )
	{
		ui.coinsTable->removeRow(current/*-1*/);//������� ������� �� ��������
		update();
		showCoin(current);
		ui.coinsTable->selectRow(current);
	}
	//���� ������� ������ ������ � ������ �� ����
	else if( current!=count-1 && count!=0 && current==0 )
	{
		ui.coinsTable->removeRow(current);	//������� ������� �� ��������
		update();
		showCoin(0);
		ui.coinsTable->selectRow(0);
	}
	//���� ������� ������ ��������� � ������ �� ����
	else if( current==count-1 && count>1 )
	{
		ui.coinsTable->removeRow(current);	//������� ������� �� ��������
		update();
		showCoin(count-1);
		ui.coinsTable->selectRow(count-1);
	}
	//���� ��������� ������
	else if( count==1 )
	{
		ui.coinsTable->clearContents();	//������� ������� �������
		ui.deleteB->setEnabled(false);
		hide(false);					//������������ ������� ��������������
	}
	remove = false;
}

/*!������� ��������� �������� ������ ��� ������ ��������������� �������� ������
*/
void client::saveName()
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	//���� ������ ��������
	if(ui.coinsTable->currentRow()>-1 && !remove && !addition && !filling)
	{
		int current=ui.coinsTable->currentRow();
		QTableWidgetItem * cur_rec;
		cur_rec = ui.coinsTable->item(current,3);
		int Id_ = cur_rec->text().toInt();
		DWORD bytesread,byteswritten;
		int name=0;
		int len;
		QString shift=ui.NameOfCoinLine->text();
		char char_shift[2000]={NULL};
		len=Str_toChar(shift,char_shift);
		request_number=14;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int),		&byteswritten, NULL);
		long int request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int),		&byteswritten, NULL);
		long int Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&name,			 sizeof(int),		&byteswritten, NULL);
		long int name_right = (long int) name;
		name_right = htonl(name_right);
		send(sock_new,(char *)&name_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&len,			 sizeof(int),		&byteswritten, NULL);
		long int len_right = (long int) len;
		len_right = htonl(len_right);
		send(sock_new,(char *)&len_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&char_shift,	 sizeof(char)*len,	&byteswritten, NULL);
		send(sock_new,char_shift,sizeof(char)*len,0);

		//������ � ����������
		int insertPos;
		request_number=17;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int),		&byteswritten, NULL);
		request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int),		&byteswritten, NULL);
		Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//ReadFile (hPipe, (void*)&insertPos,		 sizeof(int),		&bytesread,	   NULL);
		recv(sock_new,(char *)&insertPos, sizeof(long int),0);
		insertPos = ntohl(insertPos);

		update();
		ui.coinsTable->selectRow(insertPos);
	}
}

/*!������� ��������� ������� ��� ������ ��������������� �������� ������
*/
void client::saveNom(/*QString*/)
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	//���� ������ ��������
	if(ui.coinsTable->currentRow()>-1 && !remove && !addition && !filling)
	{
		int current=ui.coinsTable->currentRow();
		QTableWidgetItem * cur_rec;
		cur_rec = ui.coinsTable->item(current,3);
		int Id_ = cur_rec->text().toInt();
		DWORD byteswritten;
		int name=1;
		int len;
		QString shift=ui.NominalLine->text();
		char char_shift[2000]={NULL};
		len=Str_toChar(shift,char_shift);
		request_number=14;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
		long int request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
		long int Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&name,			 sizeof(int), &byteswritten, NULL);
		long int name_right = (long int) name;
		name_right = htonl(name_right);
		send(sock_new,(char *)&name_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&len,			 sizeof(int), &byteswritten, NULL);
		long int len_right = (long int) len;
		len_right = htonl(len_right);
		send(sock_new,(char *)&len_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&char_shift,	 sizeof(char)*len, &byteswritten, NULL);
		send(sock_new,char_shift,sizeof(char)*len,0);
	}
}

/*!������� ��������� ��� ��� ��������� �������� �������� ��������������� �������
*\param  [in]  t - �������� ��� ������������ � ��������
*/
void client::saveEra(QListWidgetItem *)
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	//���� ������ ��������
	if(ui.coinsTable->currentRow()>-1 && !remove && !addition && !filling)
	{
		int current=ui.coinsTable->currentRow();
		QTableWidgetItem * cur_rec;
		cur_rec = ui.coinsTable->item(current,3);
		int Id_ = cur_rec->text().toInt();
		DWORD bytesread,byteswritten;
		int name=0;
		int shift=ui.EraList->currentRow();
		request_number=15;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
		long int request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
		long int Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&name,			 sizeof(int), &byteswritten, NULL);
		long int name_right = (long int) name;
		name_right = htonl(name_right);
		send(sock_new,(char *)&name_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&shift,			 sizeof(int), &byteswritten, NULL);
		long int shift_right = (long int) shift;
		shift_right = htonl(shift_right);
		send(sock_new,(char *)&shift_right,sizeof(long int),0);
		
		//������ � ����������
		int insertPos;
		request_number=17;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
		request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
		Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//ReadFile (hPipe, (void*)&insertPos,		 sizeof(int), &bytesread,NULL);
		recv(sock_new,(char *)&insertPos, sizeof(long int),0);
		insertPos = ntohl(insertPos);


		update();
		ui.coinsTable->selectRow(insertPos);
	}
}

/*!������� ��������� ��� ������� ��� ��������� �������� ��������������� �������
*\param  [in]  t - �������� ��� ������������ � ��������
*/
void client::saveYear(/*int t*/)
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	//���� ������ ��������
	if(ui.coinsTable->currentRow()>-1 && !remove && !addition && !filling)
	{
		int current=ui.coinsTable->currentRow();
		QTableWidgetItem * cur_rec;
		cur_rec = ui.coinsTable->item(current,3);
		int Id_ = cur_rec->text().toInt();
		DWORD bytesread,byteswritten;
		int name=1;
		int shift=ui.YearLine->value();
		request_number=15;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
		long int request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
		long int Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&name,			 sizeof(int), &byteswritten, NULL);
		long int name_right = (long int) name;
		name_right = htonl(name_right);
		send(sock_new,(char *)&name_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&shift,			 sizeof(int), &byteswritten, NULL);
		long int shift_right = (long int) shift;
		shift_right = htonl(shift_right);
		send(sock_new,(char *)&shift_right,sizeof(long int),0);
		
		//������ � ����������
		int insertPos;
		request_number=17;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
		request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
		Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//ReadFile (hPipe, (void*)&insertPos,		 sizeof(int), &bytesread,NULL);
		recv(sock_new,(char *)&insertPos, sizeof(long int),0);
		insertPos = ntohl(insertPos);

		update();
		ui.coinsTable->selectRow(insertPos);
	}
}

/*!������� ��������� ��� ��� ��������� �������� ��������������� �������
*\param  [in]  t - �������� ��� ������������ � ��������
*/
void client::saveCent(/*int t*/)
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	//���� ������ ��������
	if(ui.coinsTable->currentRow()>-1 && !remove && !addition && !filling)
	{
		int current=ui.coinsTable->currentRow();
		QTableWidgetItem * cur_rec;
		cur_rec = ui.coinsTable->item(current,3);
		int Id_ = cur_rec->text().toInt();
		DWORD bytesread,byteswritten;
		int name=2;
		int shift=ui.CenturyLine->value();
		request_number=15;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
		long int request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
		long int Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&name,			 sizeof(int), &byteswritten, NULL);
		long int name_right = (long int) name;
		name_right = htonl(name_right);
		send(sock_new,(char *)&name_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&shift,			 sizeof(int), &byteswritten, NULL);
		long int shift_right = (long int) shift;
		shift_right = htonl(shift_right);
		send(sock_new,(char *)&shift_right,sizeof(long int),0);
		
		//������ � ����������
		int insertPos;
		request_number=17;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
		request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
		Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//ReadFile (hPipe, (void*)&insertPos,		 sizeof(int), &bytesread,NULL);
		recv(sock_new,(char *)&insertPos, sizeof(long int),0);
		insertPos = ntohl(insertPos);

		update();
		ui.coinsTable->selectRow(insertPos);
	}
}

/*!������� ��������� ������� ��� ��������� ��������� ������ ���������������� �������
*\param  [in]  t - �������� ��� ������������ � ��������
*/
void client::saveRari(bool)
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	//���� ������ ��������
	if(ui.coinsTable->currentRow()>-1 && !remove && !addition && !filling)
	{
		int current=ui.coinsTable->currentRow();
		QTableWidgetItem * cur_rec;
		cur_rec = ui.coinsTable->item(current,3);
		int Id_ = cur_rec->text().toInt();
		DWORD bytesread,byteswritten;
		int name=2;
		int len;
		QString shift=("");
		if(ui.RarityF->isChecked())
			shift="1";
		else
			shift="0";
		char char_shift[2000]={NULL};
		len=Str_toChar(shift,char_shift);
		request_number=14;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
		long int request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
		long int Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&name,			 sizeof(int), &byteswritten, NULL);
		long int name_right = (long int) name;
		name_right = htonl(name_right);
		send(sock_new,(char *)&name_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&len,			 sizeof(int), &byteswritten, NULL);
		long int len_right = (long int) len;
		len_right = htonl(len_right);
		send(sock_new,(char *)&len_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&char_shift,	 sizeof(char)*len, &byteswritten, NULL);
		send(sock_new,char_shift,sizeof(char)*len,0);
		
		int insertPos;
		request_number=17;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
		request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
		Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//ReadFile (hPipe, (void*)&insertPos,		 sizeof(int), &bytesread,NULL);
		recv(sock_new,(char *)&insertPos, sizeof(long int),0);
		insertPos = ntohl(insertPos);

		update();
		ui.coinsTable->selectRow(insertPos);
	}
}

/*!������� ��������� ���-�� ������������� ��� ��������� �������� �������� �������
*\param  [in]  t - �������� ��� ������������ � ��������
*/
void client::saveSPC(QListWidgetItem *)
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	//���� ������ ��������
	if(ui.coinsTable->currentRow()>-1 && !remove && !addition && !filling)
	{
		int current=ui.coinsTable->currentRow();
		QTableWidgetItem * cur_rec;
		cur_rec = ui.coinsTable->item(current,3);
		int Id_ = cur_rec->text().toInt();
		DWORD byteswritten;
		int name=4;
		int shift=ui.SPClist->currentRow();
		request_number=15;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
		long int request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
		long int Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&name,			 sizeof(int), &byteswritten, NULL);
		long int name_right = (long int) name;
		name_right = htonl(name_right);
		send(sock_new,(char *)&name_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&shift,			 sizeof(int), &byteswritten, NULL);
		long int shift_right = (long int) shift;
		shift_right = htonl(shift_right);
		send(sock_new,(char *)&shift_right,sizeof(long int),0);
	}
}

/*!������� ��������� ���-�� ���������� ����� ��� ��������� �������� �������
*\param  [in]  t - �������� ��� ������������ � ��������
*/
void client::saveCOSC(/*int t*/)
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	//���� ������ ��������
	if(ui.coinsTable->currentRow()>-1 && !remove && !addition && !filling)
	{
		int current=ui.coinsTable->currentRow();
		QTableWidgetItem * cur_rec;
		cur_rec = ui.coinsTable->item(current,3);
		int Id_ = cur_rec->text().toInt();
		DWORD byteswritten;
		int name=3;
		int shift=ui.COSCline->value();
		request_number=15;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
		long int request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
		long int Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&name,			 sizeof(int), &byteswritten, NULL);
		long int name_right = (long int) name;
		name_right = htonl(name_right);
		send(sock_new,(char *)&name_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&shift,			 sizeof(int), &byteswritten, NULL);
		long int shift_right = (long int) shift;
		shift_right = htonl(shift_right);
		send(sock_new,(char *)&shift_right,sizeof(long int),0);
		update();
	}
}

/*!������� ��������� �������� ��� ������ ��������������� �������� ������
*/
void client::saveFeat()
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	//���� ������ ��������
	if(ui.coinsTable->currentRow()>-1 && !remove && !addition && !filling)
	{
		int current=ui.coinsTable->currentRow();
		QTableWidgetItem * cur_rec;
		cur_rec = ui.coinsTable->item(current,3);
		int Id_ = cur_rec->text().toInt();
		DWORD byteswritten;
		int name=3;
		int len;
		QString shift=ui.FeatureLine->text();
		char char_shift[2000]={NULL};
		len=Str_toChar(shift,char_shift);
		request_number=14;
		//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
		long int request_number_right = (long int) request_number;
		request_number_right=htonl(request_number_right);
		send(sock_new,(char *)&request_number_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&Id_,			 sizeof(int), &byteswritten, NULL);
		long int Id_right = (long int) Id_;
		Id_right = htonl(Id_right);
		send(sock_new,(char *)&Id_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&name,			 sizeof(int), &byteswritten, NULL);
		long int name_right = (long int) name;
		name_right = htonl(name_right);
		send(sock_new,(char *)&name_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&len,			 sizeof(int), &byteswritten, NULL);
		long int len_right = (long int) len;
		len_right = htonl(len_right);
		send(sock_new,(char *)&len_right,sizeof(long int),0);
		//WriteFile(hPipe, (void*)&char_shift,	 sizeof(char)*len, &byteswritten, NULL);
		send(sock_new,char_shift,sizeof(char)*len,0);
	}
}

/*!������� ��������� ������� ������� ��������
*/
void client::fillCoin()
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		return ;
	}
	CloseHandle(fileH);

	hide(true);//���������� ������� ��������������
	DWORD byteswritten,bytesread;

	filling = false;
	//��������� ������� ������� ��������
	Coin f1;
	f1.NameOfCoin = "ruble";
	f1.Nominal = "100 kopecks";
	f1.Era[0]=1;
	f1.Era[1]=0;
	f1.Year = 1281;
	f1.Century = 1;
	f1.isRarity = false;
	f1.COSC = 1;
	f1.SPC[0]=1;
	f1.SPC[1]=0;
	f1.SPC[2]=0;
	f1.Feature = "The ruble is one of the oldest national currencies in Europe";

	request_number=12;
	//������ � ���������� ������
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	long int request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	QString str = ("");
	str=toStr(f1);
	char c_str[2000]={NULL};
	int len;
	long int Id_;
	len=Str_toChar(str,c_str);
	//WriteFile(hPipe, (void*)&len, sizeof(int), &byteswritten, NULL);
	long int len_right = (long int) len;
	len_right=htonl(len_right);
	send(sock_new,(char *)&len_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&c_str, sizeof(char)*len, &byteswritten, NULL);
	send(sock_new,c_str,sizeof(char)*len,0);
	//ReadFile (hPipe, (void*)&Id_,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&Id_, sizeof(long int),0);
	Id_ = ntohl(Id_);

	Coin f2;
	f2.NameOfCoin = "kopeck";
	f2.Nominal = "1 kopeck";
	f2.Era[0]=1;
	f2.Era[1]=0;
	f2.Year = 1535;
	f2.Century = 1;
	f2.isRarity = false;
	f2.COSC = 1;
	f2.SPC[0]=1;
	f2.SPC[1]=0;
	f2.SPC[2]=0;
	f2.Feature = "A kopeck saved is a ruble earned";

	request_number=12;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	str = ("");
	str=toStr(f2);
	len=Str_toChar(str,c_str);
	//WriteFile(hPipe, (void*)&len, sizeof(int), &byteswritten, NULL);
	len_right = (long int) len;
	len_right=htonl(len_right);
	send(sock_new,(char *)&len_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&c_str, sizeof(char)*len, &byteswritten, NULL);
	send(sock_new,c_str,sizeof(char)*len,0);
	//ReadFile (hPipe, (void*)&Id_,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&Id_, sizeof(long int),0);
	Id_ = ntohl(Id_);

	Coin f3;
    f3.NameOfCoin = "hryvnia";
	f3.Nominal = "100 kopecks";
	f3.Era[0]=1;
	f3.Era[1]=0;
	f3.Year = 1992;
	f3.Century = 1;
	f3.isRarity = false;
	f3.COSC = 1;
	f3.SPC[0]=0;
	f3.SPC[1]=1;
	f3.SPC[2]=0;
	f3.Feature = "Had three meanings: as a mark of distinction, known as weight and as a coin";
	
	request_number=12;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	str = ("");
	str=toStr(f3);
	len=Str_toChar(str,c_str);
	//WriteFile(hPipe, (void*)&len, sizeof(int), &byteswritten, NULL);
	len_right = (long int) len;
	len_right=htonl(len_right);
	send(sock_new,(char *)&len_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&c_str, sizeof(char)*len, &byteswritten, NULL);
	send(sock_new,c_str,sizeof(char)*len,0);
	//ReadFile (hPipe, (void*)&Id_,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&Id_, sizeof(long int),0);
	Id_ = ntohl(Id_);

	Coin f4;
	f4.NameOfCoin = "mark";
	f4.Nominal = "20 shillings";
	f4.Era[0]=1;
	f4.Era[1]=0;
	f4.Year = 1870;
	f4.Century = 1;
	f4.isRarity = false;
	f4.COSC = 1;
	f4.SPC[0]=1;
	f4.SPC[1]=0;
	f4.SPC[2]=0;
	f4.Feature = "One of the major currency in the German states";
	
	request_number=12;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	str = ("");
	str=toStr(f4);
	len=Str_toChar(str,c_str);
	//WriteFile(hPipe, (void*)&len, sizeof(int), &byteswritten, NULL);
	len_right = (long int) len;
	len_right=htonl(len_right);
	send(sock_new,(char *)&len_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&c_str, sizeof(char)*len, &byteswritten, NULL);
	send(sock_new,c_str,sizeof(char)*len,0);
	//ReadFile (hPipe, (void*)&Id_,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&Id_, sizeof(long int),0);
	Id_ = ntohl(Id_);

	Coin f5;
	f5.NameOfCoin = "euro";
	f5.Nominal = "100 cents";
	f5.Era[0]=1;
	f5.Era[1]=0;
	f5.Year = 1999;
	f5.Century = 1;
	f5.isRarity = false;
	f5.COSC = 1;
	f5.SPC[0]=0;
	f5.SPC[1]=0;
	f5.SPC[2]=1;
	f5.Feature = "The official currency of 17 countries 'euro'";
	
	request_number=12;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	str = ("");
	str=toStr(f5);
	len=Str_toChar(str,c_str);
	//WriteFile(hPipe, (void*)&len, sizeof(int), &byteswritten, NULL);
	len_right = (long int) len;
	len_right=htonl(len_right);
	send(sock_new,(char *)&len_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&c_str, sizeof(char)*len, &byteswritten, NULL);
	send(sock_new,c_str,sizeof(char)*len,0);
	//ReadFile (hPipe, (void*)&Id_,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&Id_, sizeof(long int),0);
	Id_ = ntohl(Id_);

	Coin f6;
	f6.NameOfCoin = "dollar";
	f6.Nominal = "100 cents";
	f6.Era[0]=1;
	f6.Era[1]=0;
	f6.Year = 1861;
	f6.Century = 1;
	f6.isRarity = false;
	f6.COSC = 1;
	f6.SPC[0]=0;
	f6.SPC[1]=0;
	f6.SPC[2]=1;
	f6.Feature = "The U.S. dollar is the main reserve currency of the world";

	request_number=12;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	str = ("");
	str=toStr(f6);
	len=Str_toChar(str,c_str);
	//WriteFile(hPipe, (void*)&len, sizeof(int), &byteswritten, NULL);
	len_right = (long int) len;
	len_right=htonl(len_right);
	send(sock_new,(char *)&len_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&c_str, sizeof(char)*len, &byteswritten, NULL);
	send(sock_new,c_str,sizeof(char)*len,0);
	//ReadFile (hPipe, (void*)&Id_,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&Id_, sizeof(long int),0);
	Id_ = ntohl(Id_);

	Coin f7;
	f7.NameOfCoin = "penny";
	f7.Nominal = "2 kopecks";
	f7.Era[0]=1;
	f7.Era[1]=0;
	f7.Year = 1654;
	f7.Century = 1;
	f7.isRarity = false;
	f7.COSC = 1;
	f7.SPC[0]=1;
	f7.SPC[1]=0;
	f7.SPC[2]=0;
	f7.Feature = "Be pennyless";
	
	request_number=12;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	str = ("");
	str=toStr(f7);
	len=Str_toChar(str,c_str);
	//WriteFile(hPipe, (void*)&len, sizeof(int), &byteswritten, NULL);
	len_right = (long int) len;
	len_right=htonl(len_right);
	send(sock_new,(char *)&len_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&c_str, sizeof(char)*len, &byteswritten, NULL);
	send(sock_new,c_str,sizeof(char)*len,0);
	//ReadFile (hPipe, (void*)&Id_,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&Id_, sizeof(long int),0);
	Id_ = ntohl(Id_);

	Coin f8;
	f8.NameOfCoin = "livre";
	f8.Nominal = "20 sue";
	f8.Era[0]=1;
	f8.Era[1]=0;
	f8.Year = 1799;
	f8.Century = 1;
	f8.isRarity = false;
	f8.COSC = 1;
	f8.SPC[0]=1;
	f8.SPC[1]=0;
	f8.SPC[2]=0;
	f8.Feature = "Monetary unit of France";
	
	request_number=12;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	str = ("");
	str=toStr(f8);
	len=Str_toChar(str,c_str);
	//WriteFile(hPipe, (void*)&len, sizeof(int), &byteswritten, NULL);
	len_right = (long int) len;
	len_right=htonl(len_right);
	send(sock_new,(char *)&len_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&c_str, sizeof(char)*len, &byteswritten, NULL);
	send(sock_new,c_str,sizeof(char)*len,0);
	//ReadFile (hPipe, (void*)&Id_,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&Id_, sizeof(long int),0);
	Id_ = ntohl(Id_);

	Coin f9;
	f9.NameOfCoin = "penny";
	f9.Nominal = "1 penny";
	f9.Era[0]=1;
	f9.Era[1]=0;
	f9.Year = 1971;
	f9.Century = 1;
	f9.isRarity = false;
	f9.COSC = 1;
	f9.SPC[0]=0;
	f9.SPC[1]=1;
	f9.SPC[2]=0;
	f9.Feature = "The British base coin";
	
	request_number=12;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	str = ("");
	str=toStr(f9);
	len=Str_toChar(str,c_str);
	//WriteFile(hPipe, (void*)&len, sizeof(int), &byteswritten, NULL);
	len_right = (long int) len;
	len_right=htonl(len_right);
	send(sock_new,(char *)&len_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&c_str, sizeof(char)*len, &byteswritten, NULL);
	send(sock_new,c_str,sizeof(char)*len,0);
	//ReadFile (hPipe, (void*)&Id_,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&Id_, sizeof(long int),0);
	Id_ = ntohl(Id_);

	Coin f10;
	f10.NameOfCoin = "shilling";
	f10.Nominal = "5 pences";
	f10.Era[0]=1;
	f10.Era[1]=0;
	f10.Year = 1502;
	f10.Century = 1;
	f10.isRarity = false;
	f10.COSC = 1;			
	f10.SPC[0]=1;
	f10.SPC[1]=0;
	f10.SPC[2]=0;
	f10.Feature = "In England, as in common people 'bob'";
	
	request_number=12;
	//WriteFile(hPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	str = ("");
	str=toStr(f10);
	len=Str_toChar(str,c_str);
	//WriteFile(hPipe, (void*)&len, sizeof(int), &byteswritten, NULL);
	len_right = (long int) len;
	len_right=htonl(len_right);
	send(sock_new,(char *)&len_right,sizeof(long int),0);
	//WriteFile(hPipe, (void*)&c_str, sizeof(char)*len, &byteswritten, NULL);
	send(sock_new,c_str,sizeof(char)*len,0);
	//ReadFile (hPipe, (void*)&Id_,sizeof(int),&bytesread,NULL);
	recv(sock_new,(char *)&Id_, sizeof(long int),0);
	Id_ = ntohl(Id_);

    update();
	ui.coinsTable->selectRow(0);	//�������� ������ ������ �� ����������
	showCoin(0);
	ui.deleteB->setEnabled(true);				//���������� ������ �������� ������
	filling = false;
}

void client::openEvent()
{
	WSADATA wsaData;
	int iResult;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sock_new=socket(AF_INET,SOCK_STREAM, 0);
	char IP_cstr[100]={NULL};
	int len=Str_toChar(IP_str,IP_cstr);
	sockaddr_in sain;sain.sin_family=AF_INET;sain.sin_port=htons(port_f);sain.sin_addr.s_addr=inet_addr(IP_cstr);
	sockaddr_in sai_;sai_.sin_family=AF_INET;sai_.sin_port=htons(port_s);sai_.sin_addr.s_addr=inet_addr(IP_cstr);
	iResult=::connect(sock_new,(SOCKADDR*)&sain, sizeof(sain));
	if(iResult == SOCKET_ERROR) {
        QMessageBox::critical(this,"Error!","SOCKET doesn't exist");
		closesocket(sock_new);
		WSACleanup();
        return;
	}
	Sleep(1000);
	sock_upd=socket(AF_INET,SOCK_STREAM,0);
	iResult=::connect(sock_upd,(SOCKADDR*)&sai_, sizeof(sai_));
	if(iResult == SOCKET_ERROR) {
        QMessageBox::critical(this,"Error!","SOCKET doesn't exist");
		closesocket(sock_upd);
		WSACleanup();
        return;
	}

	ui.fillB	->show();
	ui.addB		->show();
	ui.deleteB	->show();

	ui.goB		->hide();
	ui.portfLine->hide();
	ui.portsLine->hide();
	ui.IPLine	->hide();
	ui.label_pf ->hide();
	ui.label_ps ->hide();
	ui.label_IP	->hide();

	update();
	if(count>0){
		ui.deleteB->setEnabled(true);
		hide(true);
	}
}

void client::closeEvent(QCloseEvent * event)
{
	USES_CONVERSION;
	DWORD bytes;
	HANDLE fileH = CreateFileA("C:\\Users\\Tex\\Desktop\\Client-server Net\\server\\server\\Error.log", 
		GENERIC_READ | GENERIC_WRITE, 
		NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char error[] = "Terminated!";
	char adopted[30] = {NULL};
	int ln=strlen(error);
	ReadFile(fileH, (void*)&adopted, sizeof(char)*ln, &bytes, NULL);
	if(strcmp(adopted,error)==0){
		QMessageBox::critical(this,"Error!","Server is disconnected!");
		CloseHandle(fileH);
		closesocket(sock_new);
		closesocket(sock_upd);
		WSACleanup();
		return ;
	}
	CloseHandle(fileH);
	DWORD byteswritten;
	request_number=19;
	//WriteFile(hPipe,	(void*)&request_number, sizeof(int), &byteswritten, NULL);
	long int request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	//WriteFile(updPipe,	(void*)&request_number, sizeof(int), &byteswritten, NULL);
	request_number_right = (long int) request_number;
	request_number_right=htonl(request_number_right);
	send(sock_new,(char *)&request_number_right,sizeof(long int),0);
	closesocket(sock_new);
	closesocket(sock_upd);
	WSACleanup();
	event->accept();
}

Coin client::toCoin_()
{
	USES_CONVERSION;
	int i=0,j=0,buffer=0;
	Coin temp;
	DWORD bytesread;
	long int size;
	char c_str[1000]={NULL};
	//ReadFile(updPipe,(void*)&size,sizeof(int),&bytesread,NULL);
	recv(sock_upd,(char *)&size, sizeof(long int),0);
	size = ntohl(size);
	//ReadFile(updPipe,(void*)&c_str,sizeof(char)*size,&bytesread,NULL);
	recv(sock_upd,c_str, sizeof(char)*size,0);
	QString str=(c_str);
	QString one=("1");
	QString tmp=("");

	buffer=str.indexOf(";",j+1);
	temp.NameOfCoin=tmp.insert(0,str.mid(j,buffer-j));tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.Nominal=tmp.insert(0,str.mid(j+1,buffer-j));tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.Era[0]=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.Era[1]=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.Year=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.Century=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	tmp.insert(0,str.mid(j+1,buffer-j));
	if(tmp==one)
		temp.isRarity=true;
	else
		temp.isRarity=false;
	tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.COSC=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.SPC[0]=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.SPC[1]=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();
	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.SPC[2]=tmp.insert(0,str.mid(j+1,buffer-j)).toInt();tmp.clear();

	j=str.indexOf(";",j+1);
	buffer=str.indexOf(";",j+1)-1;
	temp.Feature=tmp.insert(0,str.mid(j+1,buffer-j));tmp.clear();
	j=str.indexOf(";",j+1);
	i=str.indexOf("|",i+1);
	if(i == -1)
		return temp;
	buffer=i-j-1;
	temp.Id=tmp.insert(0,str.mid(j+1,buffer)).toInt();tmp.clear();

	return temp;
}

void client::updateClient()
{
	USES_CONVERSION;
	DWORD bytesread,byteswritten;
	long int request_number_upd=20;
	//WriteFile(updPipe, (void*)&request_number, sizeof(int), &byteswritten, NULL);
	//long int request_number_right = (long int) request_number;
	request_number_upd=htonl(request_number_upd);
	send(sock_upd,(char *)&request_number_upd,sizeof(long int),0);
	long int solution;
	//ReadFile(updPipe,(void*)&solution,sizeof(int),&bytesread,NULL);
	recv(sock_upd,(char *)&solution, sizeof(long int),0);
	solution = ntohl(solution);
	if(solution == 1){
		//ReadFile(updPipe,(void*)&count,sizeof(int),&bytesread,NULL);
		recv(sock_upd,(char *)&count, sizeof(long int),0);
		count = ntohl(count);
		ui.coinsTable->setRowCount(count);			//������������� ���������� ������� � ��������
		QTableWidgetItem *item;						//������ � �������

		//������� ��� ������ � �������
		for (int i=0;i<count;++i){
			Coin temp;
			temp=toCoin_();
			Sleep(500);

			item = new  QTableWidgetItem(temp.NameOfCoin);//������ ����� ������
			ui.coinsTable->setItem(i, 0, item);	//� ������ �������

			//���� ���� ���,
			if(temp.Era[0]==1){
				if(temp.Year>=0 && temp.Year<10){
					item = new QTableWidgetItem(temp.YearInStr.number(temp.Year).insert(0,"000"));
				}
				else if(temp.Year>9 && temp.Year<100){
					item = new QTableWidgetItem(temp.YearInStr.number(temp.Year).insert(0,"00"));
				}
				else if(temp.Year>99 && temp.Year<1000){
					item = new QTableWidgetItem(temp.YearInStr.number(temp.Year).insert(0,"0"));
				}
				else if(temp.Year>999 && temp.Year<2012){
					item = new QTableWidgetItem(temp.YearInStr.number(temp.Year));
				}
				ui.coinsTable->setItem(i, 1, item);//�� ������ �������
			}
			//���� �� ����� ���
			else if(temp.Era[1]==1){
				if(temp.Century>=1 && temp.Century<10){
					item = new QTableWidgetItem(temp.CenturyInString.number(temp.Century).insert(2," BC"));
				}
				else if(temp.Century>9 && temp.Century<=21){
					item = new QTableWidgetItem(temp.CenturyInString.number(temp.Century).insert(2," BC"));
				}
				ui.coinsTable->setItem(i, 1, item);//�� ������ �������
			}

			//���� ������ ����������
			if(temp.isRarity){
				item = new QTableWidgetItem(temp.COSCinString.number(temp.COSC));//������ COSC
				ui.coinsTable->setItem(i ,2, item);//� ������ �������
			}
			//����� ���� ������ �� ����������
			else if(!temp.isRarity){
				item = new QTableWidgetItem("no rarities");//������ ������ ����������� "no rarities"
				ui.coinsTable->setItem(i ,2, item);
			}
			QString IdStr = IdStr.number(temp.Id);
			item = new QTableWidgetItem(IdStr);
			ui.coinsTable->setItem(i ,3, item);
		}
	}
}