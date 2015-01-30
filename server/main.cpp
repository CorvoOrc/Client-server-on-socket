/* Steshenko A.S. IVT-265 */

#include <QtCore/QCoreApplication>
#include <WinSock2.h>
#include <stdio.h>
#include "coin.h"
#include "database.h"
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <string>
#include <QString>
#include <vector>
#include <windows.h>
#include <ATLCONV.H>

#define _count	  10
#define _clear	  11
#define _append   12
#define _remove   13
#define _save_s   14
#define _save_i   15
#define _record   16
#define _sort	  17
#define db_to_str 18
#define ending    19
#define update	  20
#define stop	  21

using namespace std;
DataBase base;
vector <pair <SOCKET,bool> >   change_cl;
HANDLE hAdd,hRem,hCount,hSave_i,hSave_s,
hRec,hSort,hDb,hClear,hEnd;
HANDLE hC,hUpd,hEndUpd;
HANDLE hExit;
HANDLE hTimer,hTimerEvent,hTimerExit;

int Global = 0;
int max_size_waitconn = 4;

void  Fill_DataBase(){
	HANDLE fh=CreateFileA((LPCSTR)"DataBase.txt",GENERIC_READ | GENERIC_WRITE, NULL, NULL,
	OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	char write_str[1000]={NULL};
	DWORD bytesread;
	ReadFile(fh,(void*)&write_str,sizeof(char)*1000,&bytesread,NULL);
	int i=0,j=0,index=0,buffer=0;
	Coin temp;
	QString str(write_str);
	QString one=("1");
	QString tmp=("");
	int count_rec=str.count("|");
	if(count_rec>0){
		while(index<count_rec){
			if(index>0)
				j=i+1;
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
			i=str.indexOf("|",i+1);
			buffer=i-j-1;
			temp.Feature=tmp.insert(0,str.mid(j+1,buffer));tmp.clear();
			base.append(temp);
			++index;
		}
	}
	CloseHandle(fh);
}

DWORD WINAPI Mult ( LPVOID adds){
	bool first_req=true;
	DWORD bytesread, byteswritten;
	SOCKET sock_new = (SOCKET) adds;
	long int request_number;
	Coin temp;
	QString str=("");
	QString one=("1");
	QString tmp=("");
	int size,count_handle;
	HANDLE hCountGroup[5];
	HANDLE hClearGroup[12];
	HANDLE hAddGroup[13];
	HANDLE hRemGroup[13];
	HANDLE hSave_iGroup[9];
	HANDLE hSave_sGroup[9];
	HANDLE hRecGroup[5];
	HANDLE hSortGroup[12];
	HANDLE hDbGroup[7];
	HANDLE hEnd_Group[2];

    while(1){
		//DWORD retVal = ReadFile(hPipe,(void*)&request_number,sizeof(int),&bytesread,NULL);
		recv(sock_new,(char *)&request_number, sizeof(long),0);
		request_number = ntohl(request_number);

		switch(request_number){
		case _count:{
			hCountGroup[0]=hAdd,
			hCountGroup[1]=hRem,
			hCountGroup[2]=hSort,
			hCountGroup[3]=hClear,
			hCountGroup[4]=hExit;

			WaitForMultipleObjects(5,hCountGroup,1,INFINITE);
			ResetEvent(hCount);
			printf("EnterCriticalSectionCount\n");
				int bytes=base.count();
				//WriteFile(hPipe, (void*)&bytes, sizeof(int), &byteswritten, NULL);
				long int bytes_right = (long int) bytes;
				bytes_right=htonl(bytes_right);
				send(sock_new,(char *)&bytes_right,sizeof(long int),0);
				printf("request number: %d\n",_count);
				printf("request:_count\n");
				printf("count = %d\n",bytes);
			SetEvent(hCount);
			printf("LeaveCriticalSectionCount\n\n");
			break;
					}
		case _clear:{
			hClearGroup[0] =hAdd,
			hClearGroup[1] =hRem,
			hClearGroup[2] =hCount,
			hClearGroup[3] =hSave_i,
			hClearGroup[4] =hSave_s,
			hClearGroup[5] =hRec,
			hClearGroup[6] =hSort,
			hClearGroup[7] =hDb,
			hClearGroup[8] =hClear,
			hClearGroup[9] =hTimerEvent,
			hClearGroup[10]=hUpd,
			hClearGroup[11]=hExit;

			WaitForMultipleObjects(12,hClearGroup,1,INFINITE);
			ResetEvent(hClear);
			printf("EnterCriticalSectionClear\n");
				base.clear();
				printf("request number: %d\n",_clear);
				printf("request: _clear\n");
			SetEvent(hClear);
			printf("LeaveCriticalSectionClear\n\n");
			break;
					}
		case _append:{
			int i=0,j=0,buffer=0,Id_;
			Coin temp;
			DWORD bytesread;
			long int size;
			char c_str[1000]={NULL};
			//ReadFile(hPipe,(void*)&size,sizeof(int),&bytesread,NULL);
			recv(sock_new,(char *)&size, sizeof(long),0);
			size = ntohl(size);
			//ReadFile(hPipe,(void*)&c_str,sizeof(char)*size,&bytesread,NULL);
			recv(sock_new,c_str, sizeof(char)*size,0);
			hAddGroup[0] =hAdd,
			hAddGroup[1] =hRem,
			hAddGroup[2] =hCount,
			hAddGroup[3] =hSave_i,
			hAddGroup[4] =hSave_s,
			hAddGroup[5] =hRec,
			hAddGroup[6] =hSort,
			hAddGroup[7] =hDb,
			hAddGroup[8] =hClear,
			hAddGroup[9] =hUpd,
			hAddGroup[10]=hEndUpd,
			hAddGroup[11]=hTimerEvent,
			hAddGroup[12]=hExit;

			WaitForMultipleObjects(13,hAddGroup,1,INFINITE);
			ResetEvent(hAdd);
			printf("EnterCriticalSectionApp\n");
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
				i=str.indexOf("|",i+1);
				buffer=i-j-1;
				temp.Feature=tmp.insert(0,str.mid(j+1,buffer));tmp.clear();

				Id_ = base.append(temp);
				//WriteFile(hPipe, (void*)&Id_, sizeof(int), &byteswritten, NULL);
				long int Id_right = (long int) Id_;
				Id_right=htonl(Id_right);
				send(sock_new,(char *)&Id_right,sizeof(long int),0);
				int size_cl = change_cl.size();
				for(int ii=0;ii<size_cl;++ii)
					 change_cl[ii].second=true;
				printf("request number: %d\n",_append);
				printf("request: _append\n");
				printf("%s\n",c_str);
			//Освободить критическую секцию 
			SetEvent(hAdd);
			printf("LeaveCriticalSectionApp\n\n");
			break;
					 }
		case _remove:{
			int i;
			long int Id_;
			//ReadFile(hPipe,(void*)&Id_,sizeof(int),&bytesread,NULL);
			recv(sock_new,(char *)&Id_, sizeof(long int),0);
			Id_ = ntohl(Id_);
			hRemGroup[0] =hAdd,
			hRemGroup[1] =hRem,
			hRemGroup[2] =hCount,
			hRemGroup[3] =hSave_i,
			hRemGroup[4] =hSave_s,
			hRemGroup[5] =hRec,
			hRemGroup[6] =hSort,
			hRemGroup[7] =hDb,
			hRemGroup[8] =hClear,
			hRemGroup[9] =hUpd,
			hRemGroup[10]=hEndUpd,
			hRemGroup[11]=hTimerEvent,
			hRemGroup[12]=hExit;

			WaitForMultipleObjects(13,hRemGroup,1,INFINITE);
			ResetEvent(hRem);
			printf("EnterCriticalSectionRem\n");
				i = base.searchId((int)Id_);
				if(i == -1){
					printf("request number: %d\n",_remove);
					printf("request: _remove\n");
					printf("Record no exist\n");
					SetEvent(hRem);
					printf("LeaveCriticalSectionRem\n\n");
					break;
				}
				base.remove(i);
				int size_cl = change_cl.size();
				for(int ii=0;ii<size_cl;++ii)
					 change_cl[ii].second=true;
				printf("request number: %d\n",_remove);
				printf("request: _remove\n");
				printf("i = %d\n",i);
			SetEvent(hRem);
			printf("LeaveCriticalSectionRem\n\n");
			break;
					 }
		case _save_s:{
			int i;
			long int Id_,field,size;
			char char_shift[2000]={NULL};
			//ReadFile(hPipe,(void*)&Id_,sizeof(int),&bytesread,NULL);
			recv(sock_new,(char *)&Id_, sizeof(long int),0);
			Id_ = ntohl(Id_);
			//ReadFile(hPipe,(void*)&field,sizeof(int),&bytesread,NULL);
			recv(sock_new,(char *)&field, sizeof(long int),0);
			field = ntohl(field);
			//ReadFile(hPipe,(void*)&size,sizeof(int),&bytesread,NULL);
			recv(sock_new,(char *)&size, sizeof(long int),0);
			size = ntohl(size);
			//ReadFile(hPipe,(void*)&char_shift,sizeof(char)*size,&bytesread,NULL);
			recv(sock_new,char_shift, sizeof(char)*size,0);
			hSave_sGroup[0]=hAdd,
			hSave_sGroup[1]=hRem,
			hSave_sGroup[2]=hSort,
			hSave_sGroup[3]=hDb,
			hSave_sGroup[4]=hClear,
			hSave_sGroup[5]=hUpd,
			hSave_sGroup[6]=hEndUpd,
			hSave_sGroup[7]=hTimerEvent,
			hSave_sGroup[8]=hExit;

			WaitForMultipleObjects(9,hSave_sGroup,1,INFINITE);
			ResetEvent(hSave_s);
			printf("EnterCriticalSectionEd_s\n");
				QString shift=(char_shift);
				i = base.searchId((int)Id_);
				if(i == -1){
					printf("request number: %d\n",_save_s);
					printf("request: _save_s\n");
					printf("Record no exist\n");
					SetEvent(hSave_s);
					printf("LeaveCriticalSectionEd_s\n\n");
					break;
				}
				base.save(i,(int)field,shift);
				int size_cl = change_cl.size();
				for(int ii=0;ii<size_cl;++ii)
					 change_cl[ii].second=true;
				printf("request number: %d\n",_save_s);
				printf("request: _save_s\n");
				if(field == 0)
					printf("Name\n");
				else if(field == 1)
					printf("Nom\n");
				else if(field == 2)
					printf("Raritet\n");
				else if(field == 3)
					printf("Feat\n");
				printf("i = %d shift = %s\n",i,char_shift);
			SetEvent(hSave_s);
			printf("LeaveCriticalSectionEd_s\n\n");
			break;
					 }
		case _save_i:{
			int i;
			long int Id_,field,shift;
			//ReadFile(hPipe,(void*)&Id_,sizeof(int),&bytesread,NULL);
			recv(sock_new,(char *)&Id_,	  sizeof(long int),0);
			Id_ = ntohl(Id_);
			//ReadFile(hPipe,(void*)&field,sizeof(int),&bytesread,NULL);
			recv(sock_new,(char *)&field, sizeof(long int),0);
			field = ntohl(field);
			//ReadFile(hPipe,(void*)&shift,sizeof(int),&bytesread,NULL);
			recv(sock_new,(char *)&shift, sizeof(long int),0);
			shift = ntohl(shift);
			hSave_iGroup[0]=hAdd,
			hSave_iGroup[1]=hRem,
			hSave_iGroup[2]=hSort,
			hSave_iGroup[3]=hDb,
			hSave_iGroup[4]=hClear,
			hSave_iGroup[5]=hUpd,
			hSave_iGroup[6]=hEndUpd,
			hSave_iGroup[7]=hTimerEvent,
			hSave_iGroup[8]=hExit;

			WaitForMultipleObjects(9,hSave_iGroup,1,INFINITE);
			ResetEvent(hSave_i);
			
			printf("EnterCriticalSectionEd_i\n");
				i = base.searchId((int)Id_);
				if(i == -1){
					printf("request number: %d\n",_save_i);
					printf("request: _save_i\n");
					printf("Record no exist\n");
					SetEvent(hSave_i);
					printf("LeaveCriticalSectionEd_i\n\n");
					break;
				}
				
				base.save(i,(int)field,(int)shift);
				int size_cl = change_cl.size();
				for(int ii=0;ii<size_cl;++ii)
					 change_cl[ii].second=true;
				printf("request number: %d\n",_save_i);
				printf("request: _save_i\n");
				if(field == 0)
					printf("Era\n");
				else if(field == 1)
					printf("Year\n");
				else if(field == 2)
					printf("Centure\n");
				else if(field == 3)
					printf("COSC\n");
				else if(field == 4)
					printf("SPC\n");
				printf("i = %d shift = %d\n",i,shift);
			SetEvent(hSave_i);
			printf("LeaveCriticalSectionEd_i\n\n");
			break;
					 }
		case _record:{
			long int Id_;
			//ReadFile(hPipe,(void*)&Id_,sizeof(int),&bytesread,NULL);
			recv(sock_new,(char *)&Id_, sizeof(long),0);
			Id_ = ntohl(Id_);
			hRecGroup[0]=hAdd,
			hRecGroup[1]=hRem,
			hRecGroup[2]=hSort,
			hRecGroup[3]=hClear,
			hRecGroup[4]=hExit;

			WaitForMultipleObjects(5,hRecGroup,1,INFINITE);
			ResetEvent(hRec);
			printf("EnterCriticalSectionRec\n");
				int index = base.searchId((int)Id_);
				int index_right;
				if(index == -1){
					//WriteFile(hPipe, (void*)&index,	sizeof(int),&byteswritten, NULL);
					index_right = (long int)index;
					index_right=htonl(index_right);
					send(sock_new,(char *)&index_right,sizeof(long int),0);
					printf("request number: %d\n",_record);
					printf("request: _record\n");
					printf("Record no exist\n");
					SetEvent(hRec);
					printf("LeaveCriticalSectionRec\n\n");
					break;
				}
				str = base.record(index);
				QByteArray ba;
				char * rec;
				int size;
				ba=str.toAscii();
				rec=ba.data();
				size=str.size();
				char clear_str[2000]={NULL};
				for(int i=0;i<size;++i)
					clear_str[i]=rec[i];

				//WriteFile(hPipe, (void*)&index,		sizeof(int),		&byteswritten, NULL);
				index_right = (long int)index;
				index_right=htonl(index_right);
				send(sock_new,(char *)&index_right,sizeof(long int),0);
				//WriteFile(hPipe, (void*)&size,		sizeof(int),		&byteswritten, NULL);
				long int size_right = (long int)size;
				size_right=htonl(size_right);
				send(sock_new,(char *)&size_right,sizeof(long int),0);
				//WriteFile(hPipe, (void*)&clear_str, sizeof(char)*size,  &byteswritten, NULL);
				send(sock_new,clear_str,sizeof(char)*size,0);
				str.clear();
				printf("request number: %d\n",_record);
				printf("request: _record\n");
				printf("%s\n",clear_str);
			SetEvent(hRec);
			printf("LeaveCriticalSectionRec\n\n");
			break;
					 }
		case _sort:{
			long int Id_,insertPos;
			//ReadFile(hPipe,(void*)&Id_,sizeof(int),&bytesread, NULL);
			recv(sock_new,(char *)&Id_, sizeof(long int),0);
			Id_ = ntohl(Id_);
			hSortGroup[0] =hAdd,
			hSortGroup[1] =hRem,
			hSortGroup[2] =hCount,
			hSortGroup[3] =hSave_i,
			hSortGroup[4] =hSave_s,
			hSortGroup[5] =hRec,
			hSortGroup[6] =hSort,
			hSortGroup[7] =hDb,
			hSortGroup[8] =hClear,
			hSortGroup[9] =hUpd,
			hSortGroup[10]=hTimerEvent,
			hSortGroup[11]=hExit;

			WaitForMultipleObjects(12,hSortGroup,1,INFINITE);
			ResetEvent(hSort);
			printf("EnterCriticalSectionSort\n");
				insertPos=base.sort((int)Id_);
				//WriteFile(hPipe, (void*)&insertPos, sizeof(int), &byteswritten, NULL);
				long int insertPos_right = (long int)insertPos;
				insertPos_right=htonl(insertPos_right);
				send(sock_new,(char *)&insertPos_right,sizeof(long int),0);
			SetEvent(hSort);
			printf("LeaveCriticalSectionSort\n\n");
			break;
				   }
		case db_to_str:{
			str=("");
			QByteArray ba;
			char * rec;
			int size;
			hDbGroup[0]=hAdd,
			hDbGroup[1]=hRem,
			hDbGroup[2]=hSave_i,
			hDbGroup[3]=hSave_s,
			hDbGroup[4]=hSort,
			hDbGroup[5]=hClear,
			hDbGroup[5]=hExit;

			WaitForMultipleObjects(7,hDbGroup,1,INFINITE);
			ResetEvent(hDb);
			printf("EnterCriticalSectionDb\n");
				printf("request number: %d\n",db_to_str);
				printf("request: db_to_str\n");
				int count_rec_ = base.count();
				for(int i = 0;i<count_rec_;++i){
					str = base.record_str(i);
					int len = str.length();
					str[len-1] = ';';
					str.append(base.getId(i));
					str.append("|");
					ba=str.toAscii();
					rec=ba.data();
					size=str.size();
					char clear_str[2000]={NULL};
					for(int i_=0;i_<size;++i_)
						clear_str[i_]=rec[i_];
					printf("%s\n",clear_str);
					
					//WriteFile(hPipe, (void*)&size, sizeof(int), &byteswritten, NULL);
					long int size_right = (long int)size;
					size_right=htonl(size_right);
					send(sock_new,(char *)&size_right,sizeof(long int),0);
					//WriteFile(hPipe, (void*)&clear_str, sizeof(char)*size, &byteswritten, NULL);
					send(sock_new,clear_str,sizeof(char)*size,0);
					str.clear();
				}
			SetEvent(hDb);
			printf("LeaveCriticalSectionDb\n\n");
			break;
					   }
		case ending:{
			hEnd_Group[0] = hEnd,
			hEnd_Group[1] = hExit;
			WaitForMultipleObjects(2,hEnd_Group,1,INFINITE);
			ResetEvent(hEnd);
				printf("\nFinish!\n\n");
				--Global;
				closesocket(sock_new);
			SetEvent(hEnd);
			return EXIT_SUCCESS;
					}
		}
	}
}

DWORD WINAPI Upd  ( LPVOID adds){
	DWORD bytesread, byteswritten;
	SOCKET sock_upd = (SOCKET) adds;
	HANDLE hUpdGroup[9];
	HANDLE hEndUpdGroup[7];
	long int request_number=0;
	while(1){
		//DWORD retVal = ReadFile(updPipe,(void*)&request_number,sizeof(int),&bytesread,NULL);
		recv(sock_upd,(char *)&request_number, sizeof(long int),0);
		request_number = ntohl(request_number);

		switch(request_number){
		case update:{
			hUpdGroup[0]=hAdd,
			hUpdGroup[1]=hRem,
			hUpdGroup[2]=hSave_i,
			hUpdGroup[3]=hSave_s,
			hUpdGroup[4]=hSort,
			hUpdGroup[5]=hClear,
			hUpdGroup[6]=hUpd,
			hUpdGroup[7]=hEndUpd,
			hUpdGroup[8]=hExit;

			WaitForMultipleObjects(9,hUpdGroup,1,INFINITE);
			ResetEvent(hUpd);
				long int size = change_cl.size(), solution, index=0,solution_right;
				QByteArray ba;
				char * rec;
				bool fg = false;
				for(int i=0;i<size;++i)
					if(change_cl[i].first == sock_upd && change_cl[i].second == true)
						fg = true,index=i;
				if(fg){
					change_cl[index].second=false;
					solution = 1;
					//WriteFile(updPipe, (void*)&solution, sizeof(int), &byteswritten, NULL);
					solution_right = (long int) solution;
					solution_right=htonl(solution_right);
					send(sock_upd,(char *)&solution_right,sizeof(long int),0);

					printf("update to client... \n");
					QString str=("");
					int count_rec=base.count();
					//WriteFile(updPipe, (void*)&count_rec, sizeof(int), &byteswritten, NULL);
					long int count_rec_right = (long int) count_rec;
					count_rec_right=htonl(count_rec_right);
					send(sock_upd,(char *)&count_rec_right,sizeof(long int),0);

					for(int i = 0;i<count_rec;++i){
						str = base.record_str(i);
						int len = str.length();
						str[len-1] = ';';
						str.append(base.getId(i));
						str.append("|");
						ba=str.toAscii();
						rec=ba.data();
						size=str.size();
						char clear_str[2000]={NULL};
						for(int i_=0;i_<size;++i_)
							clear_str[i_]=rec[i_];
						//WriteFile(updPipe, (void*)&size, sizeof(int), &byteswritten, NULL);
						long int size_right = (long int) size;
						size_right=htonl(size_right);
						send(sock_upd,(char *)&size_right,sizeof(long int),0);
						//WriteFile(updPipe, (void*)&clear_str, sizeof(char)*size, &byteswritten, NULL);
						send(sock_upd,clear_str,sizeof(char)*size,0);
						str.clear();
					}
					printf("update to complite \n\n");
				}
				else{
					solution = 0;
					//WriteFile(updPipe, (void*)&solution, sizeof(int), &byteswritten, NULL);
					solution_right = (long int) solution;
					solution_right=htonl(solution_right);
					send(sock_upd,(char *)&solution_right,sizeof(long int),0);
				}
			SetEvent(hUpd);
			break;
					}
		case ending:{
			hEndUpdGroup[0]=hAdd,
			hEndUpdGroup[1]=hRem,
			hEndUpdGroup[2]=hSave_i,
			hEndUpdGroup[3]=hSave_s,
			hEndUpdGroup[4]=hUpd,
			hEndUpdGroup[5]=hEndUpd,
			hEndUpdGroup[6]=hExit;
			WaitForMultipleObjects(7,hEndUpdGroup,1,INFINITE);
			ResetEvent(hEndUpd);
				closesocket(sock_upd);
				pair <SOCKET, bool> temp,temp_;
				temp.first  = sock_upd, temp.second = false,
				temp_.first = sock_upd, temp_.second = true;
				vector<pair<SOCKET,bool> >::iterator it = change_cl.begin();
				while(*it!=temp && *it!=temp_)
					++it;
				change_cl.erase(it);
			SetEvent(hEndUpd);
			return EXIT_SUCCESS;
					}
		}
	}
}
DWORD WINAPI MainThread ( LPVOID pl){
	//1) socked
	SOCKET sock  = socket(AF_INET,SOCK_STREAM,0);
	SOCKET sock_ = socket(AF_INET,SOCK_STREAM,0);
	//2) bind
	HANDLE fileH = CreateFileA("Config.txt", GENERIC_READ | GENERIC_WRITE, NULL, NULL,OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,NULL);
	char cport[100] = {NULL};
	DWORD bytesread;
	int port_f,port_s;
	ReadFile(fileH,(void*)&cport,sizeof(char)*100,&bytesread,NULL);
	QString port = cport,tmp_str="";
	int index = port.indexOf(":", 0);
	int index_= port.indexOf(";",0);
	tmp_str = port.mid(index+1, index_-index-1);
	port_f = tmp_str.toInt();
	index = port.indexOf(":", index_);
	index_= port.indexOf(";",index);
	tmp_str = port.mid(index+1, index_-index-1);
	port_s = tmp_str.toInt();
	CloseHandle(fileH);
	cout<<endl<<"port for servicing:"<<port_f<<endl<<"port for update:"<<port_s<<endl;

	sockaddr_in sain;sain.sin_family=AF_INET;sain.sin_port=htons((short int)port_f);sain.sin_addr.s_addr=inet_addr("127.0.0.1");//INADDR_ANY;
	sockaddr_in sai_;sai_.sin_family=AF_INET;sai_.sin_port=htons((short int)port_s);sai_.sin_addr.s_addr=inet_addr("127.0.0.1");//INADDR_ANY;
	bind(sock, (SOCKADDR*)&sain,sizeof(sockaddr_in));
	bind(sock_,(SOCKADDR*)&sai_,sizeof(sockaddr_in));
	//3) listen
	listen(sock, max_size_waitconn);
	listen(sock_,max_size_waitconn);
	//4) accept
	while (1){
		if(Global<2){
			WaitForSingleObject(hC,INFINITE);
			ResetEvent(hC);
			sockaddr_in sain_c;
			SOCKET sock_new=accept(sock,(SOCKADDR*)&sain_c,NULL);
			if (sock_new == INVALID_SOCKET){
				printf("accept failed with error: %ld\n", WSAGetLastError());
				closesocket(sock_new);
				SetEvent(hC);
				WSACleanup();
				break;
			}
			SOCKET sock_upd=accept(sock_,NULL,NULL);
			if (sock_upd == INVALID_SOCKET){
				printf("accept failed with error: %ld\n", WSAGetLastError());
				closesocket(sock_upd);
				SetEvent(hC);
				WSACleanup();
				break;
			}
			if(sock_new != INVALID_SOCKET && sock_upd != INVALID_SOCKET){
				printf("Client join\n");
				HANDLE new_th = CreateThread(NULL, 0,Mult ,(LPVOID*)sock_new,0,0);
				HANDLE upd_th = CreateThread(NULL, 0,Upd  ,(LPVOID*)sock_upd,0,0);
				pair <SOCKET, bool> temp;
				temp.first = sock_upd,temp.second = false;
				change_cl.push_back(temp);
				++Global;
				SetEvent(hC);
			}
			else{
				SetEvent(hC);
				break;
			}
		}
	}
	return EXIT_SUCCESS;
}

void  FuncExit(){
	HANDLE hExitGroup[13];
	hExitGroup[0] =hAdd,
	hExitGroup[1] =hRem;
	hExitGroup[2] =hCount,
	hExitGroup[3] =hSave_i,
	hExitGroup[4] =hSave_s;
	hExitGroup[5] =hRec,
	hExitGroup[6] =hSort,
	hExitGroup[7] =hDb,
	hExitGroup[8] =hEnd,
	hExitGroup[9] =hClear,
	hExitGroup[10]=hUpd,
	hExitGroup[11]=hEndUpd,
	hExitGroup[12]=hTimerEvent,
	hExitGroup[13]=hExit;

	WaitForMultipleObjects(14,hExitGroup,1,INFINITE);
	ResetEvent(hExit);
	HANDLE fh=CreateFileA((LPCSTR)"DataBase.txt", GENERIC_READ | GENERIC_WRITE, NULL, NULL,CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,NULL);

	QString str=("");
	QByteArray ba;
	char * rec;
	int size;
	int count_rec_ = base.count();
	for(int i = 0;i<count_rec_;++i)
		str.append(base.record(i)),str.append("|");
	ba=str.toAscii();
	rec=ba.data();
	size=str.size();
	char clear_str[2000]={NULL};
	for(int i=0;i<size;++i)
		clear_str[i]=rec[i];
	DWORD byteswrited;
	WriteFile(fh, (void*)&clear_str, sizeof(char)*size, &byteswrited, NULL);
	CloseHandle(fh);
	base.clear();
	CloseHandle(hTimer);

	HANDLE fileH = CreateFileA("Error.log", GENERIC_READ | GENERIC_WRITE, NULL, NULL,CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,NULL);
	char error[] = "Terminated!";
	int len=strlen(error);
	DWORD bytes;
	WriteFile(fileH, (void*)&error, sizeof(char)*len, &bytes, NULL);
	CloseHandle(fileH);
	WSACleanup();
	printf("ending programm...");
	Sleep(1000);
	ExitProcess(0);
}

void  Active(){
	DWORD bytes;
	HANDLE fileH = CreateFileA("Error.log", GENERIC_READ | GENERIC_WRITE, NULL, NULL,CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,NULL);
	char error[] = "Active";
	int len=strlen(error);
	WriteFile(fileH, (void*)&error, sizeof(char)*len, &bytes, NULL);
	CloseHandle(fileH);
}


VOID  CALLBACK Rewrite(LPVOID lpArg, DWORD dwTimerLowValue,DWORD dwTimerHighValue){
	HANDLE TimerGroup[7];
	TimerGroup[0] = hAdd,
	TimerGroup[1] = hRem,
	TimerGroup[2] = hSave_i,
	TimerGroup[3] = hSave_s,
	TimerGroup[4] = hSort,
	TimerGroup[5] = hClear,
	TimerGroup[6] = hExit;
	WaitForMultipleObjects(7,TimerGroup,1,INFINITE);
	ResetEvent(hTimerEvent);
	printf("Start Timer\n");
	HANDLE fh=CreateFileA((LPCSTR)"DataBase.txt", GENERIC_READ | GENERIC_WRITE, NULL, NULL,CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,NULL);
	QString str=("");
	QByteArray ba;
	char * rec;
	int size;
	int count_rec_ = base.count();
	for(int i = 0;i<count_rec_;++i)
		str.append(base.record(i)),str.append("|");
	ba=str.toAscii();
	rec=ba.data();
	size=str.size();
	char clear_str[2000]={NULL};
	for(int i=0;i<size;++i)
		clear_str[i]=rec[i];
	DWORD byteswrited;
	WriteFile(fh, (void*)&clear_str, sizeof(char)*size, &byteswrited, NULL);
	CloseHandle(fh);
	SetEvent(hTimerEvent);
	printf("Ending Timer\n\n");
}
void  InitializationEvents(){
	hAdd	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"foradd");
	hRem	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"forrem");
	hCount	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"forcount");
	hSave_i	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"forsavei");
	hSave_s	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"forsaves");
	hRec	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"forrec");
	hSort	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"forsort");
	hDb		=CreateEvent(NULL,TRUE,true,(LPCWSTR)"fordb");
	hClear	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"forclear");
	hEnd	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"forend");
	hC		=CreateEvent(NULL,TRUE,true,(LPCWSTR)"forconnect");
	hUpd	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"forupdate");
	hEndUpd	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"forendupdate");
	hTimerEvent	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"fortimer");
	hExit	=CreateEvent(NULL,TRUE,true,(LPCWSTR)"forstopserver");

	hTimer = CreateWaitableTimer( NULL, false, (LPCTSTR)"my_timer");
	LARGE_INTEGER liDueTime;
    liDueTime.QuadPart=-100000000;
	bool ctrl = SetWaitableTimer (
		hTimer,
		&liDueTime,
		6000,
		Rewrite,
		NULL,
		0);
}

DWORD WINAPI StopServer( LPVOID pl){
	HANDLE stopPipe = CreateNamedPipeA("\\\\.\\pipe\\stopserver",
	   PIPE_ACCESS_DUPLEX,
	   PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,
	   254,
	   1024,
	   1024,
	   5000,
	   NULL);
	if(stopPipe == INVALID_HANDLE_VALUE)
	   return EXIT_SUCCESS;
	bool connect_stopPipe = ConnectNamedPipe(stopPipe,NULL);
	if( connect_stopPipe ){
		DWORD bytesread, byteswritten;
		int request_number;
		while(1){
			DWORD retVal = ReadFile(stopPipe,(void*)&request_number,sizeof(int),&bytesread,NULL);
			if(retVal && bytesread!=sizeof(int)){
				printf("ERROR! Invalid number of bytes to read!");
				break;
			}
			if(request_number == stop){
				FuncExit();
				break;
			}
		}
	}
	else
		printf("Connection failed\n");
	 return EXIT_SUCCESS;
}
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	USES_CONVERSION;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	HANDLE mut = CreateMutex ( NULL, FALSE, (LPCWSTR)"server_active");
	if(GetLastError() == ERROR_ALREADY_EXISTS){
		printf("server is already active!");
		Sleep(1000);
		return 0;
	}
	DWORD bytesread, byteswritten;
	printf("start programm ...");
	Fill_DataBase();
	Active();
	InitializationEvents();
	HANDLE connection;

	//создаем новый поток
	HANDLE th = CreateThread(NULL,0,MainThread,(LPVOID*)connection,0,0);
	HANDLE hStop = CreateThread(NULL,0,StopServer,(LPVOID*)connection,0,0);
	return a.exec();
}