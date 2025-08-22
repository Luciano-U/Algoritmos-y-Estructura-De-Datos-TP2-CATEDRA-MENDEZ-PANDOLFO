# Nombre del archivo de salida
OUTPUT = salida

# Archivos fuente
SRC = assets.c engine.c hash.c menu.c pokedex.c pokemones.c tp2.c \
      pila.c lista.c mapa.c juego_logica.c mapa_objetos.c juego_datos.c

# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall

# Regla principal
$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)

# Limpiar los archivos generados
clean:
	rm -f $(OUTPUT)

