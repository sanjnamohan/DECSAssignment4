#include "KVClient.h"

using namespace std;

int main(){
	int socket_fd;
	socket_fd = init_socket();
	char request_buffer[513]; 
	char operation[1];
	/* Part 1 and 2 hold key and value respectively, with padding if necessary */
	char part1[256],part2[256];
	

	/**************************START OF MESSAGE HANDLING**************************/
		
	while(1){

		cout<<"Select the operation(1/2/3):\n1) GET \n2) PUT\n3) DELETE\n";
		cin>>operation;

		if(operation[0]=='1' || operation[0]=='3'){
			char key[256]; //Actual user entered key
			cout<<"\nEnter the key: ";
			cin>>key;
			if(strlen(key)>256){
				cout<<"\nKey size should not exceed 256 bytes/chars";
			}
			else{
				int key_len = strlen(key);
				key[key_len] = '-';
				// cout<<"\n ********* MY KEY IS **********"<<key;

				strcpy(request_buffer,operation);
				strcat(request_buffer,key);
				fill(request_buffer+key_len+1,request_buffer+513,'-');

				break;
			}
		}
		else if(operation[0]=='2'){
				char key[256],value[256]; //Actual user entered key
				cout<<"\nEnter the key: ";
				cin>>key;
				if(strlen(key)>256){
					cout<<"\nKey size should not exceed 256 bytes/chars";
					continue;
				}
				cout<<"\nEnter the value: ";
				cin>>value;
				if(strlen(key)>256){
					cout<<"\nValue size should not exceed 256 bytes/chars";
					continue;
				}
				
				int key_len = strlen(key);
				key[key_len] = '-';
				// cout<<"\n ********* MY KEY IS **********"<<key;
				int val_len = strlen(value);
				value[val_len] = '-';
				// cout<<"\n ********* MY VALUE IS **********"<<value;

				strcpy(request_buffer,operation);
				strcat(request_buffer,key);
				fill(request_buffer+key_len+1,request_buffer+257,'-');
				request_buffer[strlen(request_buffer)] = '\0';	
	
				strcat(request_buffer,value);
				fill(request_buffer+257+val_len,request_buffer+513,'-');

		}
		else{
			cout<<"\nEnter a valid operation";
		}
	}

    cout<<"\nFINAL MESSAGE IS OF SIZE::"<<sizeof(request_buffer)<<" AND LEN::"<<strlen(request_buffer);

    /***********************END OF MESSAGE HANDLING**************************/

	cout<<"\nSENDING:"<<request_buffer;
	cout<<"===\n";
	write(socket_fd, request_buffer, 513);


	char response_buffer[1000];
	int len = read(socket_fd, response_buffer, 999);
	response_buffer[len] = '\0';
	cout<<response_buffer<<endl;

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
