
// gcc -pthread -o server server.c && ./server

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void *server_handler(void *fd_pointer);

int main()
{

    int server_fd, client_fd, *new_sock;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if ((server_fd) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        return 1;
    }
    int listenStatus = listen(server_fd, 3);
    if (listenStatus < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    clilen = sizeof(cliaddr);
    while ((client_fd = accept(server_fd, (struct sockaddr *)&cliaddr, &clilen)))

    {
        printf("\nTCP Server: A Client connected successfully!\n\n");

        pthread_t server_thread;
        new_sock = malloc(1);
        *new_sock = client_fd;
        pthread_create(&server_thread, NULL, server_handler, (void *)new_sock);
    }
    if ((client_fd) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    return 0;
}

void *server_handler(void *fd_pointer)
{

    int sock = *(int *)fd_pointer;
    int read_size, write_size;
    char *message;
    static char client_message[2000];
    message = " \nHello Server Handler \n";

    char buffer[1024] = {0};
    char *serverMessage = "Hello from server";

    /*
	read()
	Returns the number of bytes from the connection

	0 - indicates the end of the file
	-1 - Error 

	*/
    printf("\nTCP Server: A Client connected successfully!\n\n");
    int valread = read(sock, buffer, 1024);

    printf("%s\n\n", buffer);

    /*
	send(socket,buffer,len,flag) - sends a message on a socket
	
	socket - file
	buffer - The message to be sent
	len - The size of the message being sent
	
	Returns
	0 - Success
	-1 - Error
	*/
    int sendSuccess = send(sock, serverMessage, strlen(serverMessage), 0);
    if (sendSuccess < 0)
    {
        printf("An error occured while sending the message to the client");
    }
    printf("TCP Server: Message sent\n\n");

    free(fd_pointer);

    return 0;
}