//Zaira E Cruz
//October 26,2016
//Proxy server that listens for incoming connections from user input port.
//The proxy server then reads the inoput from the connected client and uses
//input to make GET command(2 lines). Proxy server then connects to another port
//80 to pass in this HTTP command and then receives message back. This message
//is again transferred back to the client.
//
//Additions: Once the http request is received in the server, the server checks whether the 
//http code is 200 if it is then the html page is cached intoa a file added to the list of cached
//pages with its respective file name, and finally sent to the client. 
//Else if the code isn't 200 OK then the html page is directly sent to the client without further action.
//At the end of either or options above, the server waits for the client to request yet another url and 
//proceed in the same manner. This time , however, the url is first searched in the list of cached websites,
//and if found simply given to the client as is.

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define SIZE 4096
//Error handling function.
void error(const char *msg) {
	perror(msg); 
	exit(0);
}

int main(int argc, char **argv)
{
	FILE *fileid,*list_file,*cachefile;

	char filename[40],tmp[80];
	time_t rawtime;
	struct tm *timeinfo;


    char str[80],*in_cache,buff[BUFFER_SIZE],*token,*ptt=NULL,*secptt=NULL;
    int cache_size=0;
    int listen_fd,conn_fd,x=0, sock_fd,portnum,n,m,p,o;
    char buffer[BUFFER_SIZE],recvline[BUFFER_SIZE],line[BUFFER_SIZE];
    char *message="GET / HTTP/1.1\r\nHost: %s\r\n\r\n";
     struct sockaddr_in servaddr,serv_addr;
	portnum=atoi(argv[1]);

    /* AF_INET - IPv4 IP , Type of socket, protocol*/
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    sock_fd = socket(AF_INET, SOCK_STREAM,0);

    bzero(&serv_addr,sizeof(serv_addr));
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(portnum);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr =htons(INADDR_ANY);
    serv_addr.sin_port = htons(80);


    /* Binds the above details to the socket */
	bind(listen_fd,  (struct sockaddr *) &servaddr, sizeof(servaddr));
    /* Start listening to incoming connections */
	listen(listen_fd, 20);


   	 /* Accepts an incoming connection */
	conn_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL);


	bzero(str, 256);

//keep accepting connections from the client.
while((n=read(conn_fd,str,255))>0){
	strtok(str,"\n");

	list_file=fopen("list.txt","r");
	token=NULL;

	printf("Check url\n");

	//Check if url is already in cache.
	while(fgets(tmp,sizeof(tmp),list_file)!=NULL)
	{

		printf("Check url\n");
		if(strstr(tmp,str)!=NULL)
		{
			fflush(stdout);
			token=strtok(tmp," ");

			token=strtok(NULL," ");

		fflush(stdout);
		break;
		}
	}
	strtok(token," ");
	strtok(token,"\n");

	printf("File=|%s|",token);

	if(token!=NULL)
	{
		cachefile=fopen(token,"r");
		do
		{
			fscanf(cachefile,"%s",buff);
			//secptt=strstr(buff,"</HTML>");
			ptt=strstr(buff,"</html>");
			write(conn_fd,buff,strlen(buff));
		}while(ptt==NULL);
		printf("Done reading message\n");
		fclose(cachefile);
		continue;
	}

    /*Concatenate message with given url*/
	sprintf(buffer,message,str);
    /*Get IP address from url and copy to the socket address*/
	struct hostent *client_server;
	client_server = gethostbyname(str);
	bcopy((char *)client_server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         client_server->h_length);

     	//Connect to port 80 second socket(sock_fd).
	if(connect(sock_fd,(struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
		error("ERROR connecting");
	//Write the given command to port 80 . HTTP Request.
	m=write(sock_fd, buffer, strlen(buffer));

	//Read in the message from port 80.
	p=read(sock_fd,line,50);

	//Go in here if the code for the html page is 200 OK. Will make file, write the html to the file,
	//and then send it to the client. 
	if(line[9]=='2'&&line[11]=='0')
	{
	//Make file name is code is 200 OK for the html page Using time it was accessed.
	//Then open to append mode.
	time (&rawtime);
        timeinfo=localtime(&rawtime);
        strftime(filename,sizeof(filename),"%Y%m%d%H%M%S", timeinfo);
        fileid=fopen(filename,"a+");
        list_file=fopen("list.txt","a+");

		cache_size=cache_size+1;
		printf("cache_size: %d\n",cache_size);
		if(cache_size<6){
			fwrite("\n",1,strlen("\n"),list_file);
			fwrite(str,1,strlen(str),list_file);
			fwrite(" ",1,strlen(" "),list_file);
			fwrite(filename,1,strlen(filename),list_file);
		}
		else{
			printf("More than 6 files.\n");
			
		}
		fclose(list_file);
		fwrite(line,1,strlen(line),fileid);

		char * pointer=NULL;
		char *second=NULL;

		do
		{
			read(sock_fd,recvline,sizeof(recvline));
			second=strstr(recvline,"</HTML>");
			pointer=strstr(recvline,"</html>");
			fwrite(recvline, 1 ,strlen(recvline),fileid);
			write(conn_fd,recvline,strlen(recvline));

		}while(pointer==NULL||second==NULL);

		fclose(fileid);
		bzero(str,256);
		//bzero(&serv_addr,sizeof(serv_addr));
		close(sock_fd);
		continue;
	}
	//Else if code for the html page is not 200 OK then simply send the html page 
	//to the client. 
	else
	{
		char *pointer=NULL;
		char *second=NULL;
		do
		{
			read(sock_fd,recvline, sizeof(recvline));
			second=strstr(recvline,"</HTML>");
			pointer=strstr(recvline,"</html>");
			write(conn_fd,recvline,strlen(recvline));
		}while(pointer==NULL||second==NULL);
		bzero(str,256);
		//bzero(&serv_addr,sizeof(serv_addr));
		close(sock_fd);
		continue;
	}

}

        close (conn_fd); //close the connection

	return 0;
}
