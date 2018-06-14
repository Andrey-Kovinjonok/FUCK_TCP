/*****************************************************
 * A TCP socket client that receive Partial Packet
 * and Merged Packets from TCP connection.
 * @author: ideawu
 * @link: http://www.ideawu.net/
 *****************************************************/
#include "inc.h"

int main(int argc, char **argv){
	if(argc <= 2){
		printf("Usage: %s ip port\n", argv[0]);
		exit(0);
	}

	const char *ip = argv[1];
	int port = atoi(argv[2]);

	int sock = sock_connect(ip, port);
	if(sock == -1){
		printf("error: %s\n", strerror(errno));
		exit(0);
	}
	printf("connected to %s:%d\n", ip, port);
	
	struct Buffer buf;
	buffer_init(&buf);
	
	while(1){
		// 可以优化成直接网络读到buf中，不需要tmp
		char tmp[128];
		int len = read(sock, tmp, sizeof(tmp));
		if(len <= 0){
			printf("receive %d, exit.\n", len);
			exit(0);
		}
		if(buffer_add(&buf, tmp, len) == -1){
			printf("error, too large packet!\n");
			exit(0);
		}
		
		int n = 0;
		while(1){
			char *msg = parse_packet(&buf);
			if(!msg){
				break;
			}
			n ++;
			
			printf("< %s\n", msg);
			if(n > 1){
				printf("    [Merged Packet]\n");
			}
			free(msg);
		}
	}

	return 0;
}


