//Zaira E Cruz
//September 28,2016
//Client socket code that writes. to proxy server an inputted url
//(proxy server then finds IP address of url sends request) and then client
//reads from proxy server the message received from the HTTP request.
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char **argv)
{
    int sockfd, n,portnum,m;
    int len = sizeof(struct sockaddr);
    char recvline[40960],* pointer=NULL;
    char buffer[256];
    struct sockaddr_in servaddr;

	//Ask user for url and saves into buffer char array.
	//printf("Url: ");
	//fgets(buffer,sizeof(buffer),stdin);

    /* AF_INET - IPv4 IP , Type of socket, protocol*/
    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr,sizeof(servaddr));
    //port number is taken from main arguments.
    portnum=atoi(argv[1]);

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(portnum); // Server port number

    /* Convert IPv4 and IPv6 addresses from text to binary form */
	inet_pton(AF_INET,"129.120.151.94",&(servaddr.sin_addr));

    /* Connect to the server */
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
while(1){
	//Ask user for url and saves into buffer char array.
        printf("\nUrl: ");
        fgets(buffer,sizeof(buffer),stdin);

	//socket writes the url to the server.
    n = write(sockfd, buffer, strlen(buffer));
    if(n<0)
	error("ERROR writing to socket");
	//Client then reads from server the message received from HTTP request.
	printf("Starting to receive message");
    do
    {
	read(sockfd,recvline,sizeof(recvline));
	//second=strstr(recvline,"</HTML>");
	pointer=strstr(recvline,"</html>");
	printf("%s1",recvline);
    }while(pointer==NULL);
	bzero(buffer,sizeof(buffer));
}
close(sockfd);
printf("Done receiving\n");

    return 0;
}
