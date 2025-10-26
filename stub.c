#include "stub.h"

struct message message;
struct lamport lamport;
struct sockets sockets;
pthread_mutex_t lamport_mutex = PTHREAD_MUTEX_INITIALIZER;
char process_name[20]; 
pthread_t receiver_thread; // guardamos el identificador global del hilo receptor
int finish = 0;

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

void update_lamport() {
    pthread_mutex_lock(&lamport_mutex);
    lamport.lc = max(lamport.lc, lamport.lr) + 1;
    pthread_mutex_unlock(&lamport_mutex);
}

void update_lamport_send() {
    pthread_mutex_lock(&lamport_mutex);
    lamport.lc++;
    pthread_mutex_unlock(&lamport_mutex);
}

int get_clock_lamport() {
    pthread_mutex_lock(&lamport_mutex);
    int val = lamport.lc;
    pthread_mutex_unlock(&lamport_mutex);
    return val;
}

void set_process_name(const char *name) {
    strncpy(process_name, name, sizeof(process_name) - 1);
    process_name[sizeof(process_name) - 1] = '\0';
}

void *receive_loop(void *arg) {
    int sock = *(int *)arg;
    struct message msg;
    fd_set readfds;
    struct timeval timeout;
    int selected;
    int bytes;
    int done;

    while (1) {
        pthread_mutex_lock(&lamport_mutex);
        done = finish;
        pthread_mutex_unlock(&lamport_mutex);

        if (done == 1) {
            break;
        } 

        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        selected = select(sock + 1, &readfds, NULL, NULL, &timeout);
        if (selected < 0) {
            perror("Error on select");
            break;
        }

        if (selected > 0 && FD_ISSET(sock, &readfds)) {
            bytes = recv(sock, &msg, sizeof(msg), 0);
            if (bytes < 0) {
                perror("Error on recv");
                break;
            } if (bytes == 0) {
                break;
            }

            pthread_mutex_lock(&lamport_mutex);
            lamport.lr = msg.clock_lamport;
            lamport.lc = max(lamport.lc, lamport.lr) + 1;
            int lc_actual = lamport.lc;
            pthread_mutex_unlock(&lamport_mutex);

            printf("%s, %d, RECV (%s), %d\n",
                   process_name,
                   lc_actual,
                   msg.origin,
                   msg.action);
        }
    }

    return NULL;
}

int initialize_server_connection(char *IP, char *port) {

    lamport.lc = 0;
    lamport.lr = 0;
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


    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    sockets.server_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (sockets.server_sock < 0) {
        perror("Error on accept");
        close(sockfd);
        return -1;
    }


    pthread_create(&receiver_thread, NULL, receive_loop, &sockets.server_sock);

    return 0;
}

int initialize_client_connection(char *IP, char *port) {
    lamport.lc = 0;
    lamport.lr = 0;
    char *endptr;
    long valor = strtol(port, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid Port: %s\n", port);
        return -1;
    }

    struct sockaddr_in server_addr;
    unsigned short host_port = (unsigned short)valor;

    sockets.client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sockets.client_sock < 0) {
        perror("Error on socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(host_port);

    if (connect(sockets.client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error on connect");
        close(sockets.client_sock);
        return -1;
    }

    pthread_create(&receiver_thread, NULL, receive_loop, &sockets.client_sock);

    return 0;
}

int ready_to_shutdown() {
    update_lamport_send();
    strncpy(message.origin, process_name, sizeof(message.origin) - 1);
    message.origin[sizeof(message.origin) - 1] = '\0';
    message.action = READY_TO_SHUTDOWN;
    message.clock_lamport = get_clock_lamport();
    if (send(sockets.client_sock, &message, sizeof(message), 0) <= 0) {
        perror("Error on send READY_TO_SHUTDOWN");
        return -1;
    }
    printf("%s, %d, SEND, READY_TO_SHUTDOWN\n", process_name, get_clock_lamport());
    return 0;
}

int shutdown_now() {
    update_lamport_send();
    strncpy(message.origin, process_name, sizeof(message.origin) - 1);
    message.origin[sizeof(message.origin) - 1] = '\0';
    message.action = SHUTDOWN_NOW;
    message.clock_lamport = get_clock_lamport();
    if (send(sockets.server_sock, &message, sizeof(message), 0) <= 0) {
        perror("Error on send SHUTDOWN_NOW");
        return -1;
    }
    printf("%s, %d, SEND, SHUTDOWN_NOW\n", process_name, get_clock_lamport());
    return 0;
}

int shutdown_ack() {
    update_lamport_send();
    strncpy(message.origin, process_name, sizeof(message.origin) - 1);
    message.origin[sizeof(message.origin) - 1] = '\0';
    message.action = SHUTDOWN_ACK;
    message.clock_lamport = get_clock_lamport();
    if (send(sockets.client_sock, &message, sizeof(message), 0) <= 0) {
        perror("Error on send SHUTDOWN_ACK");
        return -1;
    }
    printf("%s, %d, SEND, SHUTDOWN_ACK\n", process_name, get_clock_lamport());
    return 0;
}

void control_exit() {
    pthread_mutex_lock(&lamport_mutex);
    finish = 1;
    pthread_mutex_unlock(&lamport_mutex);
    pthread_join(receiver_thread, NULL);
}
