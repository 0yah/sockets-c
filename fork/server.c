#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{

    int server_fd, ret;
    struct sockaddr_in serverAddr;

    int client_fd;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[1024] = {0};
    char *serverMessage = "Hello from server";

    pid_t childpid;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        printf("[-]Error in connection.\n");
        exit(1);
    }

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(server_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (ret < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //Listen for connections
    /*
    int status = listen(sockid,queueLimit);
    sockid - Socket descriptor
    queuelen - No of active participants that can wait for a connection

    returns
    0 - Successful
    -1 - Error

    */
    int listenStatus = listen(server_fd, 3);
    if (listenStatus < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("\nTCP Server: Waiting for a client to connect!\n");
    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&newAddr, &addr_size);
        if (client_fd < 0)
        {
            exit(1);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        if ((childpid = fork()) == 0)
        {
            close(server_fd);

            while (1)
            {

                /*
	read()
	Returns the number of bytes from the connection

	0 - indicates the end of the file
	-1 - Error 

	*/
                printf("\nTCP Server: A Client connected successfully!\n\n");
                int valread = read(client_fd, buffer, 1024);

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
                int sendSuccess = send(client_fd, serverMessage, strlen(serverMessage), 0);
                if (sendSuccess < 0)
                {
                    printf("An error occured while sending the message to the client");
                }
                printf("TCP Server: Message sent\n\n");
            }
        }
    }

    close(client_fd);

    return 0;
}