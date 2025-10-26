# Compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -g -I.

# Archivos fuente
COMMON = stub.c
SRC_P1 = P1.c $(COMMON)
SRC_P2 = P2.c $(COMMON)

# Archivos objeto
OBJ_P1 = $(SRC_P1:.c=.o)
OBJ_P2 = $(SRC_P2:.c=.o)

# Regla por defecto
all: P1 P2

# Enlazado
P1: $(OBJ_P1)
	$(CC) $(CFLAGS) -o $@ $^

P2: $(OBJ_P2)
	$(CC) $(CFLAGS) -o $@ $^

# Compilación de cada archivo .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -f $(OBJ_P1) $(OBJ_P2) P1 P2
