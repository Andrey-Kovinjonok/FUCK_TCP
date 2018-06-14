#ifndef NET_H_
#define NET_H_

#include <arpa/inet.h>
#include <netinet/tcp.h>

int sock_server(const char *ip, int port);
int sock_accept(int serv_sock);
int sock_connect(const char *ip, int port);

int sock_server(const char *ip, int port){
	struct sockaddr_in addr;
	memset(&addr, sizeof(addr), 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons((short)port);
	inet_pton(AF_INET, ip, &addr.sin_addr);
	
	int serv_sock = 0;
	if((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		goto sock_err;
	}
	int opt = 1;
	if(setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
		goto sock_err;
	}
	if(bind(serv_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
		goto sock_err;
	}
	if(listen(serv_sock, 1024) == -1){
		goto sock_err;
	}
	
	return serv_sock;
	
sock_err:
	if(serv_sock){
		close(serv_sock);
	}
	return -1;
}

int sock_accept(int serv_sock){
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int sock = accept(serv_sock, (struct sockaddr *)&addr, &addrlen);
	if(sock == -1){
		return -1;
	}
	
	char remote_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr.sin_addr, remote_ip, sizeof(remote_ip));
	int remote_port = ntohs(addr.sin_port);
	
	printf("accept client from %s:%d\n", remote_ip, remote_port);
	
	int opt = 1;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&opt, sizeof(opt));

	return sock;
}

int sock_connect(const char *ip, int port){
	int sock;
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons((short)port);
	inet_pton(AF_INET, ip, &addr.sin_addr);

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		goto sock_err;
	}
	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
		goto sock_err;
	}
	return sock;

sock_err:
	if(sock){
		close(sock);
	}
	return -1;
}

#endif
