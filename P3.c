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
    set_process_name("P3");

    // Inicializar conexión con P2
    if (initialize_client_connection(IP, PORT) < 0) {
        fprintf(stderr, "Error al conectar con P2\n");
        exit(EXIT_FAILURE);
    }

    ready_to_shutdown();

    while (get_clock_lamport() != 9) {
        usleep(100000); // 100ms
    }

    shutdown_ack();

    while (get_clock_lamport() != 10) {
        usleep(100000); // 100ms
    }

    control_exit();

    return 0;
}
