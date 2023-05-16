#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define SA struct sockaddr
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	int sockfd, connfd, port, success=0;
	char msg_from_server[BUFFER_SIZE] = {0};
	char msg_from_client[BUFFER_SIZE] = {0},*path=argv[3], *serverip=argv[1];
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	if (argv[2]==NULL)
	{
		port = 80;
	}
	else
	{
		port = atoi(argv[2]);
	}
	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);// the argv1 gets the ip from the command line
	servaddr.sin_port = htons(port);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))!= 0) 
	{
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");
		
	
	snprintf(msg_from_client, BUFFER_SIZE, "GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n", path, serverip, port);
	printf("GET %s HTTP/1.1\r\n\r\n", path);
	if (send(sockfd, msg_from_client, strlen(msg_from_client), 0) < 0) 
	{
		perror("Error sending");
		exit(1);
	}

	//--------Recieve response header from the server   	 
	   	 
	 
		 long bytes_received = 0;
	     while (bytes_received ==0 || msg_from_server[bytes_received] != '\0') 
	     {
	        int n = recv(sockfd, msg_from_server, BUFFER_SIZE, 0);
	        if (n <= 0) {
	            perror("Error receiving message\n");
	            return -1;
	        }
	        
	        bytes_received += n;
	     } 
	     if(strstr(msg_from_server, "404"))
	     {
	     	printf("%s\n", msg_from_server);
	     	exit(0);
	     }
		 printf("\n%s\n",msg_from_server);// PRINT THE SERVER RESPONSE HEADER

		 
		 char buffer[1024]={0};
		 bytes_received = 0;
		     while (bytes_received ==0 || buffer[bytes_received] != '\0') {
			int n = recv(sockfd, buffer, BUFFER_SIZE, 0);
			if (n <= 0) {
			    perror("Error receiving message2\n");
			    return -1;
			}
		       
			bytes_received += n;
		     } 
		 
 
		 //msg_from_server[BUFFER_SIZE]={0};
		 memset(msg_from_server, 0, BUFFER_SIZE);

		 // Write the file contents to a file
		 printf("\nDownloading the file\n");
		 
		 char *filename = strrchr(path, '/') + 1;
		FILE* fp = fopen(filename, "wb");
		if (fp < 0) 
		{
			perror("open");		    
			exit(1);
		}	
	 
	  
		bytes_received=0;
	   	 while ((bytes_received = recv(sockfd, msg_from_server, BUFFER_SIZE, 0)) > 0) 
	   	 {
			 	if (fwrite(msg_from_server, 1, bytes_received, fp) == -1) 
				  {
				  	success=0;
					perror("write");
					exit(1);
				  }
				else{
					success=1;
				}
		 	
		 }
	    
    	 if (bytes_received < 0) 
    	 {	
    	 	success=0;
         	perror("recv");
        	return 1;
    	 }

		 if(success)
		 {
		 	printf("Downloading finished\n",filename);
		 }
		 
    
    
        //------------------Release/Clean/Free everything to memory---------------
   
	    fclose(fp);
	    close(sockfd);

	   

	// close the socket
	close(sockfd);
	 return 0;
}
