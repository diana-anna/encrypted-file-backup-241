# set up compiler
CC = gcc
CFLAGS  = -Wall

# the build target executable:
SERVER = dserver
CLIENT = dclient
DTCP = d_tcp

d_all: $(DTCP)
server: $(SERVER)
client: $(CLIENT)

$(SERVER): $(SERVER).c
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER).c

$(CLIENT): $(CLIENT).c
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT).c

$(DTCP): $(DTCP).c
	$(CC) $(CFLAGS) -o $(DTCP) $(DTCP).c