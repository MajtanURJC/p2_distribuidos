#include "stub.h"


int sockfd;
int client_sock;

int initialize_server_connection(char *IP, char *port) {
    char *endptr;
    long valor = strtol(port, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid Port: %s\n", port);
        return -1;
    }

    struct sockaddr_in server_addr;
    unsigned short host_port = (unsigned short)valor;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error on socket");
        return -1;
    }

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(host_port);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error on bind");
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, 2) < 0) {
        perror("Error on listen");
        close(sockfd);
        return -1;
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0) {
        perror("Error on accept");
        close(sockfd);
        return -1;
    }

    return 0;
}

int initialize_client_connection(char *IP, char *port) {
    char *endptr;
    long valor = strtol(port, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid Port: %s\n", port);
        return -1;
    }

    struct sockaddr_in server_addr;
    unsigned short host_port = (unsigned short)valor;

    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        perror("Error on socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(host_port);

    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error on connect");
        close(client_sock);
        return -1;
    }

    return 0;
}

int SHUTDOWN_NOW() {
    struct message message;
    message.action = SHUTDOWN_NOW;
}

int max(int a, int b) {
    if(a < b) {
        return b;
    } else {
        return a;
    }
}

int get_clock_lamport(int L,int Lr) {
    return max(L+1,Lr);
}