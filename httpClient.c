// example: 
// ./fileName hostName port

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int my_socket;
    int	port;
	int n;
	int res;
    struct sockaddr_in saddr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) 
	{
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
	
    port = atoi(argv[2]);
    my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (my_socket < 0) 
        error("ERROR opening socket");
		
    server = gethostbyname(argv[1]);
    if (server == NULL) 
	{
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }	
    bzero((char *) &saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
    bcopy((char *)server -> h_addr, (char *)&saddr.sin_addr.s_addr, server -> h_length);    
	
	res = connect(my_socket, (struct sockaddr *) &saddr, sizeof(saddr));
    if (res < 0) 
        error("ERROR connecting");
		
    printf("Please enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    n = write(my_socket, buffer, strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
		 
    bzero(buffer, 256);
    n = read(my_socket, buffer, 255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n", buffer);
	
    close(my_socket);
    return 0;
}
