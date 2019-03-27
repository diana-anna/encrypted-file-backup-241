# set up compiler
CC = gcc
CFLAGS  = -Wall

# the build target executable:
SERVER = dserver
CLIENT = dclient

all: $(SERVER) $(CLIENT)
server: $(SERVER)
client: $(CLIENT)

$(SERVER): $(SERVER).c
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER).c

$(CLIENT): $(CLIENT).c
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT).c