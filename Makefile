CC=gcc 
CFLAGS=-Wall -Wextra -g 
EXEC_CLIENT=./client 
EXEC_SERVER=./server

all: $(EXEC_CLIENT) $(EXEC_SERVER)

$(EXEC_CLIENT): client.c
	$(CC) $(CFLAGS) client.c -o $(EXEC_CLIENT)

$(EXEC_SERVER): server.c
	$(CC) $(CFLAGS) server.c -o $(EXEC_SERVER)

clean:
	rm -rf *.o server client