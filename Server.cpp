#include<winsock2.h>
#include<iostream>
#define DEFPORT 5050

using namespace std;

int map[32][32]={1};

int main(){
	system("title Server-Beta1");
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,2),&wsadata);
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		printf("Socket creation failed\n");
		WSACleanup();
		return 1;
	}
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEFPORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(s, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("Bind failed\n");
		closesocket(s);
		WSACleanup();
		return 1;
	}
	if (listen(s, 5) == SOCKET_ERROR) {
		printf("Listen failed\n");
		closesocket(s);
		WSACleanup();
		return 1;
	}
	SOCKET client_socket = accept(s, NULL, NULL);
	while(1){
	if (client_socket == INVALID_SOCKET) {
		printf("Accept failed\n");
		closesocket(s);
		WSACleanup();
		return 1;
	}
	char recvbuf[512];
	int bytes_received = recv(client_socket, recvbuf, sizeof(recvbuf), 0);
	if (bytes_received > 0) {
		recvbuf[bytes_received] = '\0';
		printf("Received: %s\n", recvbuf);
	}
		if(recvbuf=="map"){
			string buf;
			for(int i=0;i<=31;i++){
				for(int j=0;j<=31;j++){
					buf+=to_string(map[i][j]);
				}
			}
			char sendbuf[1024];
			strcpy(sendbuf,buf.c_str());
			send(client_socket,sendbuf,strlen(sendbuf),0);
		}else{
			int x=(recvbuf[0]-30)*10+(recvbuf[1]-30);
			int y=(recvbuf[2]-30)*10+(recvbuf[3]-30);
			map[x][y]=recvbuf[4]-30;
		}
		
	}
	closesocket(s);
	closesocket(client_socket);
	WSACleanup();
	return 0;
}
