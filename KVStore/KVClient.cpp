#include "KVClient.h"

using namespace std;

int main(){
	int socket_fd;
	socket_fd = init_socket();

	char request_buffer[100];
	while(fgets(request_buffer, 100, stdin) != NULL){
		cout<<"SENDING:"<<request_buffer;
		cout<<"===\n";
		write(socket_fd, request_buffer, strlen(request_buffer));


		char response_buffer[1000];
		int len = read(socket_fd, response_buffer, 999);
		response_buffer[len] = '\0';
		cout<<response_buffer<<endl;
	}

	return 0;
}

int init_socket(){
	int socket_fd,s;
	socket_fd = socket(AF_INET,SOCK_STREAM,0);

	struct addrinfo hints, *serverAdd;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	// hints.ai_addr = "127.0.0.1";

	s = getaddrinfo(NULL, "1234", &hints, &serverAdd);
	if (s != 0){
	        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        	exit(1);
	}
	connect(socket_fd,serverAdd->ai_addr, serverAdd->ai_addrlen);
	return socket_fd;
}
