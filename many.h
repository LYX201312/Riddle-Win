#ifndef MANY_H
#define MANY_H
#include<winsock2.h>
#include<graphics.h>
#include<ege/sys_edit.h>
#include<Windows.h>
#include<string>
#include<iostream>
#include<ws2tcpip.h>

using namespace std;

int ncx;
int ncy;
string ipname="";
WSADATA wsadata;


bool init(){
	int wres;
	wres = WSAStartup(MAKEWORD(2,2),&wsadata);
	if(wres != 0){
		MessageBox(GetForegroundWindow(),("WSA Returned Exit Status"+to_string(wres)).c_str(),"Error",MB_OK|MB_ICONERROR);
		WSACleanup();
		return false;
	}
	cleardevice();
	delay_fps(60);
	settextcolor(EGERGB(0,0,255));
	setfont(120,0,"方正姚体");
	settextjustify(CENTER_LINE,TOP_TEXT);
	outtextxy(ncx/2,0,"联机模式");
	setlinecolor(EGERGB(0,0,255));
	setlinewidth(10);
	line(0,121,ncx,121);
	setfillcolor(EGERGB(0,0,255));
	fillrect(0,0,ncx/2-200,120);
	fillrect(ncx/2+200,0,ncx,120);
	delay_fps(60);
	char buf[256];
	inputbox_getline("域名选择","请输入服务器IP/域名",buf,256);
	ipname=buf;
	delay_fps(60);
	return true;
}

struct hostent *host;
string IP="";
SOCKET s ; 


bool finder(){
	cleardevice();
	settextjustify(CENTER_LINE,CENTER_TEXT);
	outtextxy(ncx/2,ncy/2,"正在寻找服务器");
	delay_fps(60);
	host=gethostbyname(ipname.c_str());
	if(host==NULL){
		MessageBox(GetForegroundWindow(),"获取IP失败","Error",MB_OK|MB_ICONERROR);
		return false;
	}
	char **pptr;
	char str[32];
	switch (host->h_addrtype) {
	case AF_INET:
	case AF_INET6:
		//TODO
		pptr=host->h_addr_list;
		for(;*pptr!=NULL;pptr++){
			//TODO
			inet_ntop(host->h_addrtype,*pptr,str,sizeof(str));
			IP=inet_ntop(host->h_addrtype,host->h_addr_list,str,sizeof(str));
		}
		break;
	default:
		//TODO
		break;
	}
	string msg=(string)"确认信息:\nServer Name:"+host->h_name+"\n"+"First IP:"+IP+"\n是否确定加入服务器?";
	if(MessageBox(GetForegroundWindow(),msg.c_str(),"确认信息",MB_YESNO|MB_ICONQUESTION)==IDNO){
		MessageBox(GetForegroundWindow(),"已终止登录到服务器!","提示",MB_OK|MB_ICONINFORMATION);
		return false;
	}
	s=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		MessageBox(GetForegroundWindow(),"SOCKET Invalid!","Error",MB_OK|MB_ICONHAND);
		WSACleanup();
		return false;
	}
	return true;
}

int map[32][32];

void many() {
	if(!init())return;
	if(!finder())return;
	while(1){
		cleardevice();
		delay_fps(60);
		send(s,"map",3,0);
		cout<<"sended!";
		char buffer[1024];
		buffer[0]='\0';
		while(buffer[0]=='\0')recv(s,buffer,1024,0);
		for(int i=0;i<=31;i++){
			for(int j=0;j<=31;j++){
				map[i][j]=buffer[i*j]-30;
			}
		}
		for(int i=0;i<=31;i++){
			for(int j=0;j<=31;j++){
				if(map[i][j]==1)setfillcolor(EGERGB(0,255,0));
				fillrect(i*ncx/32,j*ncy/32,(i+1)*ncx/32,(j+1)*ncy/32);
			}
		}
		system("pause");
	}
	closesocket(s);
	WSACleanup();
	return;
}

#endif
