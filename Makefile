# set up compiler
CC = gcc
CFLAGS  = -Wall

# the build target executable:
DTCP = d_tcp

d_all: $(DTCP)

$(DTCP): $(DTCP).c
	$(CC) $(CFLAGS) -o $(DTCP) $(DTCP).c