#include "client.h"
#include <QtGui/QApplication>
#include <QTimer>
#include <WinSock2.h>
#include <windows.h>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	client w;
	w.show();
	return a.exec();
}
