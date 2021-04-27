// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char *clientMessage = "Hello from client";
    char buffer[1024] = {0};

    // Creating socket file descriptor
    /*

	int client_fd = socket(domain, type,protocol);

	Domain:
    AF_INET - IPv4 Protocol
    AF_INET6 - IPv6 Protocol

	Type(Communication Type):
	SOCK_STREAM - TCP (Realiable Connection Oriented)

	Protocol - Internet Protocol(IP) 0

	Returns
    0 - Success
    -1 - Error
    
    */

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if ((client_fd) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;   //Internet Protocol - AF_NET(IPv4) or AF_INET6(IPv6)
    serv_addr.sin_port = htons(PORT); //Address Port

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    /*
    int status = connect(sockid,&serverAddr,addrLen) -Establishes a connection with the server
	status - The new socket used for data-transfer
	sockid - interger, the original socket being listened on
	clientAddr - struct of the address of the active participant
	addrLen - sizeof(clientAddr)


    It is a blocking function therefore the program will not contine until a connection is made
	Communicates with the accept function on the client side
	Returns 
	0 - Successful
	-1 - Error

    */

    int connectionStatus = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connectionStatus < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    /*
	send(socket,buffer,len,flag) - sends a message on a socket
	
	socket - file
	buffer - The message to be sent
	len - The size of the message being sent
	
	Returns
	0 - Success
	-1 - Error
	*/
    int sendStatus = send(client_fd, clientMessage, strlen(clientMessage), 0);
    if (sendStatus < 0)
    {
        printf("An error occured while sending the message to the server");
        return -1;
    }
    printf("TCP Client:Message sent\n\n");
    /*
	read()
	Returns the number of bytes from the connection

	0 - indicates the end of the file
	-1 - Error 

	*/
    valread = read(client_fd, buffer, 1024);
    printf("%s\n", buffer);
    /*
	close(sockid) - Closes the connections
    int result = close(socket_id);
    return:
    0 - Successful
    -1 - Error
	
	*/
    int connectionCloseStatus = close(client_fd);
    if (connectionCloseStatus < 0)
    {
        printf("An error occured while closing the connection to the server");
        return -1;
    }
    printf("\n\nTCP Client: Connection closed\n");
    return 0;
}