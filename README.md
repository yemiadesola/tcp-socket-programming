This is a simple TCP client and server program in C language.

The client program can be used to download a file from a server. It sends a GET request to the server and receives the file contents in response. The server program can be used to serve files to clients. It listens for incoming connections and sends the requested file to the client.

To use the client program, compile it with the following command:

gcc TCPClient.c -o TCPClient

Then, run the client program with the following command:

./TCPClient <server_ip> <port> <file_path>

where:

    <server_ip> is the IP address of the server
    <port> is the port number of the server
    <file_path> is the path to the file on the server

To use the server program, compile it with the following command:

gcc TCPServer.c -o TCPServer

Then, run the server program with the following command:

./TCPServer <port>

where <port> is the port number on which the server will listen for incoming connections.

Once the server program is running, you can use the client program to download files from the server.

For example, to download the file index.html from the server at 127.0.0.1:8080, you would run the following command:

./TCPClient 127.0.0.1 8080 index.html

The client program will download the file to the current directory.

If the file does not exist on the server, the client program will print an error message and exit.

This program is a good example of how to use TCP sockets to send and receive data between two applications. It can be used as a starting point for developing more complex TCP applications, such as file transfer, chat, or gaming.
