#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#ifndef STUB_H
#define STUB_H

enum operations {
    READY_TO_SHUTDOWN = 0,
    SHUTDOWN_NOW,
    SHUTDOWN_ACK
};

struct message {
    char origin[20];
    enum operations action;
    unsigned int clock_lamport;
};

struct lamport {
    int lr;
    int lc;
}

struct sockets {
    int client_sock;
    int server_sock;
    int clients[];
}


int inicialize_connection(char *, char *);
int ready_to_shutdown();
int shutdown_now();
int shutdown_ack(); 
int get_clock_lamport(int);
int max(int, int);
void shutdown();

#endif // STUB_H
