CFLAGS = -std=c++11 -lwsock32

all: 
	g++ Client.cpp ClientSocket.cpp ClientSocket.h -o client $(CFLAGS)


test: all
	client.exe
