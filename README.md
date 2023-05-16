# tcp-socket-programming
This program is written in C Language
This program is able to run on two virtual machines. 
In this project, I use kali Linux and Ubuntu as server and client respectfully.
Ensure to configure the network adapter of both virtual machines to be on the same network

Below is the compilation and run instruction
Document Name: tcpserver
Compile instruction: gcc -o server tcpserver.c
Run: ./server 9999
Note: You can use any port of your choice. In my project I use 9999 as port number


Client Side
Document name: tcpclient.c
compilation instruction: gcc -o client tcpclient.c
Run program:  ./client [Server IP address] [server port] [file directory]. See example below
Run program: ./client 192.168.58.100 9999 /file.txt


