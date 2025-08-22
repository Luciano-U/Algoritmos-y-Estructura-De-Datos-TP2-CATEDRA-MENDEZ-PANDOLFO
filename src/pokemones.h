#ifndef POKEMONES_H_
#define POKEMONES_H_

#include <stdlib.h>
#include <stdbool.h>
#include "pokedex.h"

void mostrar_pokemon_1(const struct pokemon *p);
void mostrar_pokemon_2(const struct pokemon *p);
void mostrar_pokemon_3(const struct pokemon *p);
void mostrar_pokemon_4(const struct pokemon *p);
void mostrar_pokemon_5(const struct pokemon *p);
void mostrar_pokemon_6(const struct pokemon *p);
void mostrar_pokemon_7(const struct pokemon *p);
void mostrar_pokemon_8(const struct pokemon *p);
void mostrar_pokemon_9(const struct pokemon *p);
void mostrar_pokemon_10(const struct pokemon *p);
void mostrar_pokemon_11(const struct pokemon *p);
char *imprimir_tipo(tipo_pokemon tipo);
void mostrar_generico(const struct pokemon *p);
void imprimir_pokemon(const struct pokemon *p);
void imprimir_no_encontrado();
#endif // HASH_H_
