#include "stub.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);

    if (argc != 3) {
        fprintf(stderr, "Not enought arguments\n");
        exit(1);
    }

    char *IP = argv[1];
    char *PORT = argv[2];

    // Nombre del proceso
    set_process_name("P2");


    // Inicializar conexión con P2
    if (initialize_server_connection(IP, PORT) < 0) {
        fprintf(stderr, "Error al crear el server\n");
        exit(EXIT_FAILURE);
    }

    while (get_clock_lamport() != 2) {
        usleep(100000); // 100ms
    }

    shutdown_now();

    control_exit();

    return 0;
}
