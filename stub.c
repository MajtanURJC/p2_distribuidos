#include "stub.h"


struct message message;
struct lamport lamport;
struct sockets sockets;
lamport.lc = 0;

int initialize_server_connection(char *IP, char *port) {
    char *endptr;
    long valor = strtol(port, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid Port: %s\n", port);
        return -1;
    }

    struct sockaddr_in server_addr;
    unsigned short host_port = (unsigned short)valor;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);
    sockets.server_sock = accept(sockfd, (struct sockaddr *)&server_addr, &server_len);
    if (sockets.server_sock < 0) {
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

    sockets.client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        perror("Error on socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(host_port);

    if (connect(sockets.client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error on connect");
        close(client_sock);
        return -1;
    }

    return 0;
}

int shutdown_now(char name[]) {
    message.name = name;
    message.action = SHUTDOWN_NOW;
    message.clock_lamport = lamport.lc;
    int num = send(sockets.server_sock,message.name, strlen(name));

}

int shutdown_ack(char name[]) {
    message.name = name;
    message.action = SHUTDOWN_ACK;
    message.clock_lamport = lamport.lc;
}

int ready_to_shutdown(char name[]) {
    message.name = name;
    massage.action = READY_TO_SHUTDOWN;
    message.clock_lamport = lamport.lc
}

int max(int a, int b) {
    if(a < b) {
        return b;
    } else {
        return a;
    }
}

int get_clock_lamport() {
    return max(lc+1,lr);
}