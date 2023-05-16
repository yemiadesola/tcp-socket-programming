#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

//To display errors
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen, n, port=80, get_size; 
    pid_t chpid;
    off_t size_of_file;
    char buffer[256], client_msg[2000]={0}, header_srvr_resp[2000]={0};
    char *filenotfound = "404 Not Found";
    char *other_response = "400 Bad Request";
    struct sockaddr_in serv_addr, cli_addr;
    

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // --------------------------------------------create a new socket
    if (sockfd < 0) // if socket creation fails, exit with an error
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr)); // zero out the memory of the serv_addr struct

    if(argv[1] !=NULL)
    {
    	port=atoi(argv[1]);
    }

    // set the properties of the serv_addr struct
    serv_addr.sin_family = AF_INET; // address family is AF_INET (IPv4)
    serv_addr.sin_addr.s_addr = inet_addr("192.168.56.102"); // bind to any available address on the server machine
    serv_addr.sin_port = htons(port); // convert port number to network byte order (big-endian)

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) // ------------------------bind the socket to the serv_addr struct
        error("ERROR on binding");

    listen(sockfd, 5); // start listening for incoming connections, with a maximum queue length of 5

    clilen = sizeof(cli_addr); // get the size of the cli_addr struct

    while (1) { // keep accepting new connections indefinitely

        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); //-------------------------- accept a new connection
        if (newsockfd < 0) // if accepting the connection fails, exit with an error
            error("ERROR on accept");

        bzero(buffer, 256); // zero out the buffer

	if((chpid = fork()) == 0)
	 
	    {
	 	 	close(sockfd);
	 	
		    // --------------------------RECIEVING CLIENT REQUESTS------------------
		    get_size=recv(newsockfd, client_msg, 200, 0);	    
		    printf(" CLIENT says: %s\n", client_msg);
		    
		    //----------Retrieve the filepath from the client request
		    char f_path[1024] = {0};
		    int found = sscanf(client_msg, "GET /%s HTTP/1.1", f_path);
		    
		    if (found != 1) 
		    {
		    	perror("\n Error Parsing. Disconnecting from client");
		    	
       			snprintf(header_srvr_resp, 1024, "File path parsing Error. Repeat the request. Server has Disconnected");
     			send(newsockfd, header_srvr_resp, strlen(header_srvr_resp), 0);
		    	close(newsockfd);
		    	exit(1);
		    }

		           
		    //-------GET THE SIZE OF THE REQUESTED FILE --------
		    FILE *fd_exist = fopen(f_path, "rb");

		    if(fd_exist == NULL){
		    	
		    	printf("No such file");
	     		send(newsockfd, filenotfound, strlen(filenotfound), 0);
	     		exit(0);
		    }


		    FILE *f_stat = fopen(f_path, "rb");
		    struct stat st;
	     	if (fstat(fileno(f_stat), &st) != 0) //To check if the file mentioned path does exist along with the filesize
	     	{
	     		perror("Error getting file size");	
	     		send(newsockfd, other_response, strlen(other_response), 0);
	     		exit(0);	       		
	     	}


		    else
		    {
		    	char* header_srvr_resp;
			    int srh_size = 1024;

			    // allocate memory on the heap
			    header_srvr_resp = (char*) malloc(srh_size * sizeof(char));

			    // check if malloc() was successful
			    if (header_srvr_resp == NULL) {
			        printf("Failed to allocate memory\n");
			        return 1;
			    }

			    memset(header_srvr_resp,'\0', strlen(header_srvr_resp));
		    	size_of_file = st.st_size;	
		    	char str[20];
		    	sprintf(str, "%d", size_of_file);  
		    	char *header_msg="HTTP/1.1 200 OK\r\nContent-Type:text/plain\r\nContent-Length:N/A\r\n\r\n";
		    	snprintf(header_srvr_resp, strlen(header_msg), header_msg );
		    	printf ("srh length: %d\n", strlen(header_srvr_resp));  
		    	//header_srvr_resp[strlen(header_srvr_resp)]='\0';  
		    	send(newsockfd, header_srvr_resp, strlen(header_srvr_resp), 0);		    	
		    	
		    	memset(header_srvr_resp,'\0', strlen(header_srvr_resp));
		    	snprintf(header_srvr_resp, 1024, "FILE_SIZE:%d\r\n",size_of_file); 
		    	printf ("srh length2: %d\n", strlen(header_srvr_resp)); 
		    	send(newsockfd, header_srvr_resp, strlen(header_srvr_resp), 0);  			
		    	printf("length of header: %d", strlen(header_srvr_resp));
		    	
		    }
		    printf("\n----------Finished sending Initial header response------------\n");
		    //exit(0);	
	     	printf("--------- File size: %ld bytes ------------\n", st.st_size);
	     	//exit(0);

	     	fclose(f_stat);



	        FILE *fileptr = fopen(f_path, "rb");
			if (fileptr < 0) 
			{
		    	perror("open");		    
		    	close(newsockfd);
		    	exit(1);
			}	
			int fd = fileno(fileptr);


	        char *file_buffer = malloc(size_of_file);
	        memset(file_buffer,'\0', strlen(file_buffer));
	        long bytes_read = 0;
	        long bytes_sent = 0;

			
			while (bytes_sent < size_of_file) {
			    // read diff chunk of data from the filepath
			    int bytes_read = read(fd, file_buffer, size_of_file);
			    
			    // send the chunk to the client
			    send(newsockfd, file_buffer, bytes_read, 0);
			    
			    bytes_sent += bytes_read;
			}

				 
		 close(newsockfd);
		 //fclose(fileptr);
		 exit(0);
		 
	 
	    }
	   else {
            // parent process
            close(newsockfd);
            //exit(0);  // close the socket in the parent process
        }
    }

    close(sockfd); // close the main socket

    return 0;
}
