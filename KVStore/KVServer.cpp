#include "KVServer.h"

#define LISTENING_PORT "1234"
#define CLIENTS_PER_THREAD 10
#define CACHE_SIZE 10
#define CACHE_REPLACEMENT 1 //1 for LRU and 2 for LFU
#define THREAD_POOL_SIZE_INITIAL 10
#define THREAD_POOL_GROWTH 2

// no of events to monitor at a time through epoll
#define MAX_EVENTS 10

using namespace std;

int main(int argc, char const *argv[])
{
	
	int socket_fd = init_socket();
	int epollfd,nfds,client_fd;
	struct addrinfo clientAdd;
	struct epoll_event curr_event, events[MAX_EVENTS];

	// to create epoll instance
	epollfd = epoll_create1(0);
	if(epollfd==-1){
		perror("epoll_create1");
		exit(1);
	}

	curr_event.events = EPOLLIN;
	curr_event.data.fd = socket_fd;

	// add event for socket_fd to monitor for new connection
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, socket_fd, &curr_event) == -1){
		perror("epoll_ctl");
		exit(1);
	}

	while(1){

		// waiting for any event
		cout<<"waiting for event"<<endl;
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if(nfds==-1){
			perror("epoll_wait");
			exit(1);
		}

		for(int i=0 ; i<nfds ; i++){
			// now events has new connection request for socket fd and 
			// client fds who has a request
			if(events[i].data.fd == socket_fd){

				cout<< "New Connection to establish\n";
				client_fd = accept(socket_fd, NULL, NULL);
				if(client_fd==-1){
					perror("accept");
					exit(1);
				}

				cout<< "Connection made: client_fd="<< client_fd << endl;
				curr_event.events = EPOLLIN;
				curr_event.data.fd = client_fd;
				cout<< "Adding new client fd for epoll monitoring "<<endl;
				// adding the new client fd for epoll monitoring
				if(epoll_ctl(epollfd, EPOLL_CTL_ADD, client_fd, &curr_event) == -1){
					perror("epoll_ctl");
					exit(1);
				}
			}
			else{
				// handle client request
				pthread_t workerThread;
				pthread_create(&workerThread,NULL,&processRequest,&events[i].data.fd);
			}
		}
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

	s = getaddrinfo(NULL, LISTENING_PORT , &hints, &serverAdd);
	if (s != 0){
	        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        	exit(1);
	}
	if ( bind(socket_fd, serverAdd->ai_addr, serverAdd->ai_addrlen) != 0 ){
		perror("bind()");
		exit(1);
	}

	if ( listen(socket_fd, 10) != 0 ){
		perror("listen()");
		exit(1);
	}
	return socket_fd;
}

void *processRequest(void *fd){
	int client_fd = *(int *)fd;
	char request_buffer[1000];
	char response_buffer[50];
	while(1){
	  int len = read(client_fd, request_buffer, 999);
	  request_buffer[len] = '\0';

	  cout<<"Read "<<len<<" chars\n";
	  cout<<"===\n";
	  cout<<request_buffer;

	  sprintf(response_buffer,"Server response to client");
	 
	  write(client_fd, response_buffer, strlen(response_buffer));
	  if(request_buffer[0]=='X') break;
	}
	return NULL;
}
