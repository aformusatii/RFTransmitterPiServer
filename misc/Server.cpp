/*
    C socket server example, handles multiple clients using threads
*/

#include "Server.h"

volatile int socket_desc , client_sock , c , *new_sock;
volatile struct sockaddr_in server , client;

void *(*client_handler)(void *);
void *(*data_handler)(char[], int);

void *default_client_handler(void *);
void *server_handler(void *);

SocketServer::SocketServer() {
}

void SocketServer::init(uint16_t localPort) {
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
    	perror("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( localPort );

    //Bind
    if( bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    // Set default client handler
    client_handler = default_client_handler;

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    // Create listening thread
    pthread_t listner_thread;
    if( pthread_create( &listner_thread, NULL,  server_handler, NULL) < 0)
    {
        perror("could not create thread");
    }
}

void SocketServer::addClientListner(void *(*c_handler)(void *)) {
	client_handler = c_handler;
}

void SocketServer::addDataListner(void *(*d_handler)(char[], int)) {
	data_handler = d_handler;
}

void *server_handler(void *args) {
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        // puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = (int*) malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  client_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        //puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

	return 0;
}

void *default_client_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*) socket_desc;
    int read_size;
    char client_message[255];

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 255 , 0)) > 0 )
    {
    	//printf("\nread_size=%d", read_size);
        //fflush(stdout);

        char client_data[read_size];

        strncpy(client_data, client_message, read_size);

        data_handler(client_data, read_size);

        //Send the message back to client
        write(sock, "OK", 2);
    }

    /* if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    } */

    if(read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}
