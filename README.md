# TCP SOCKET PROGRAMMING

Introduction
TCP socket programming is a fundamental concept in network communication. It allows applications to establish a reliable connection and exchange data between a client and a server over a network. This repository aims to provide simple examples of TCP socket programming. This program is able to run on two virtual machines. In this project, I use kali Linux and Ubuntu as server and client respectfully.
Ensure to configure the network adapter of both virtual machines to be on the same network

Programming Language
The repository currently includes examples in C programming languages

Below is the compilation and run instruction
Document Name: tcpserver
Compile instruction: gcc -o server tcpserver.c
Run: ./server 9999
Note: You can use any port of your choice. In my project I use 9999 as port number
Also, the file that will be downloaded should be created into the VM with the server program


Client Side
Document name: tcpclient.c
compilation instruction: gcc -o client tcpclient.c
Run program:  ./client [Server IP address] [server port] [file directory]. See example below
Run program: ./client 192.168.58.100 9999 /file.txt


