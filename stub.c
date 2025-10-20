#include "stub.h"

int sockfd;

int inicialize_server_connection (char * IP, char * port) {

    char *endptr;
    long valor = strtol(port, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid Port: %s\n", port);
        return -1;
    }

    struct sockaddr_in sock;
    unsigned short host_port = (unsigned short)valor;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(host_port);

    sockfd = socket(sock.sin_family, SOCK_STREAM, 0);
    if (sockfd) {
        perror("Error on socker");
        return 1;
    }

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        return -1;
    }

    int bind_res = bind(sockfd, (struct sockaddr *)&sock, sizeof(sock));
    if (bind_res < 0) {
        perror("Error on bind");
        close(sockfd);
        return -1;
    }

    int listen_res = listen(sockfd, MAX_CLIENTS);
    if (listen_res < 0) {
        perror("Error on listen");
        close(sockfd);
        return -1;
    }

}

int inicialize_server_connection (char * IP, char * port) {

    char *endptr;
    long valor = strtol(port, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid Port: %s\n", port);
        return -1;
    }

    struct sockaddr_in sock;
    unsigned short host_port = (unsigned short)valor;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(host_port);

    sockfd = socket(sock.sin_family, SOCK_STREAM, 0);
    if (sockfd) {
        perror("Error on socker");
        return 1;
    }

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        return -1;
    }

    int bind_res = bind(sockfd, (struct sockaddr *)&sock, sizeof(sock));
    if (bind_res < 0) {
        perror("Error on bind");
        close(sockfd);
        return -1;
    }

    int listen_res = listen(sockfd, MAX_CLIENTS);
    if (listen_res < 0) {
        perror("Error on listen");
        close(sockfd);
        return -1;
    }

}