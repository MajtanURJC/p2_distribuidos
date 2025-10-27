#ifndef STUB_H
#define STUB_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <pthread.h>
#include <signal.h>
#include <pthread.h>

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
};

struct sockets {
    int client_sock;
    int server_sock;
    int P1_socket;
    int P3_socket;
};


int initialize_server_connection(char *IP, char *port);
int initialize_client_connection(char *IP, char *port);
int ready_to_shutdown();
int shutdown_now();
int shutdown_ack();
int get_clock_lamport();
void set_process_name(const char *name);
void update_lamport_send();
void control_exit();

#endif
