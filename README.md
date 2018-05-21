# ShellComputer
Program which we can sending msg/commands to other computers in lan.

Program must be compiled by gcc compiler

##Client makefile:

CFLAGS = -std=c++11 -lwsock32</code>
all: 
	g++ Client.cpp ClientSocket.cpp ClientSocket.h -o client $(CFLAGS)
test: all
  
I use mingw 4.8.1 with threading   

##Server
 
 To compile server use visual c++ compiler
 
##Configuration
 In file client.h change IP and PORT in define identifier.
 
##How is it work?
 
 Run Server on computers which we would like to send msg or commands, and another one (admin computer) install client.
 Join to the server and done!
  
  
