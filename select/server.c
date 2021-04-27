#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define SERVERPORT 8080
#define BUFSIZE 4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void *handle_connection(int);
int check(int exp, const char *msg);
int accept_new_connection(int server_socket);
int setup_server(short port, int backlog);

int main()
{

    int server_socket = setup_server(SERVERPORT, SERVER_BACKLOG);

    //File descriptor set
    fd_set current_sockets, ready_sockets;

    //Initailze set
    FD_ZERO(&current_sockets);
    FD_SET(server_socket, &current_sockets);

    while (1)
    {

        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("Select error");
            exit(-1);
        }

        //Check which socket is ready for reading
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {
                ///If a connection is ready
                if (i == server_socket)
                {
                    int client_socket = accept_new_connection(server_socket);
                    //Add the new connection to the set that we are watching
                    //Add to watch list
                    FD_SET(client_socket, &current_sockets);
                }
                else
                {
                    //if the socket that we want to ready and is a client
                    handle_connection(i);

                    //Remove from watch list
                    FD_CLR(i, &current_sockets);
                }
            }
        }
    }

    return 0;
}

int setup_server(short port, int backlog)
{
    int server_socket, client_socket, addr_size;
    SA_IN server_addr;
    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    int bindStatus = bind(server_socket, (SA *)&server_addr, sizeof(server_addr));
    if (bindStatus < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int listenStatus = listen(server_socket, backlog);
    if (listenStatus < 0)
    {
        perror("An error occurred while listening");
        exit(EXIT_FAILURE);
    }
    printf("\nTCP Server: Waiting for a client to connect!\n");

    return server_socket;
}

int accept_new_connection(int server_socket)
{
    int addr_size = sizeof(SA_IN);
    SA_IN client_addr;

    int client_socket = accept(server_socket, (SA *)&client_addr, (socklen_t *)&addr_size);
    if ((client_socket) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    return client_socket;
}

int check(int exp, const char *msg)
{
    if (exp == SOCKETERROR)
    {
        perror(msg);
        exit(1);
    }
    return exp;
}

void *handle_connection(int client_socket)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char *serverMessage = "Hello from server";

    int sendSuccess = send(client_socket, serverMessage, strlen(serverMessage), 0);
    if (sendSuccess < 0)
    {
        printf("An error occured while sending the message to the client");
    }
    printf("TCP Server: Message sent\n\n");

    int valread = read(client_socket, buffer, 1024);
    printf("%s\n\n", buffer);
    fflush(stdout);

    close(client_socket);
}