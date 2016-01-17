#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

class SocketServer {
public:
	SocketServer();

	void init(uint16_t localPort);
    void addClientListner(void *(*)(void *));
    void addDataListner(void *(*)(char[], int));
};
