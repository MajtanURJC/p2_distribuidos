#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

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


int inicialize_connection(char * IP, char * port);
int READY_TO_SHUTDOWN();
int SHUTDOWN_NOW();
int SHUTDOWN_ACK(); 
int get_clock_lamport();
void shutdown();