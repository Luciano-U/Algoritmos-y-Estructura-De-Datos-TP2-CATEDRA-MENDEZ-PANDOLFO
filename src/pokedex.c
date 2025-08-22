#include "pokedex.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

struct pokedex {
	FILE *archivo;

	/*
    * array dinamico de punteros pokemones ordenados por ID
    */
	struct pokemon **pokemones_por_id;
	/*
    * array dinamico de punteros pokemones ordenados por nombre
    */
	struct pokemon **pokemones_por_nombre;

	unsigned cantidad_pokemones;

	char **array_nombres_pokemones;
};

/*
 * ricibe una linea, verifica que sea un pokemon balido 
 * retorna true si es un pokemon, false si no lo es.
 */

bool verificar_pokemon(const char *linea_pokemon)
{
	int i = 0;

	while (isdigit(linea_pokemon[i])) {
		i++;
	}
	if (linea_pokemon[i] != ';')
		return false;
	i++;

	while (linea_pokemon[i] != ';') {
		i++;
	}
	if (linea_pokemon[i] != ';')
		return false;
	i++;

	if (linea_pokemon[i] != 'A' && linea_pokemon[i] != 'F' &&
	    linea_pokemon[i] != 'P' && linea_pokemon[i] != 'R' &&
	    linea_pokemon[i] != 'E' && linea_pokemon[i] != 'N' &&
	    linea_pokemon[i] != 'L') {
		return false;
	}

	i++;

	while (isdigit(linea_pokemon[i])) {
		i++;
	}
	if (linea_pokemon[i] != ';')
		return false;
	i++;
	while (isdigit(linea_pokemon[i])) {
		i++;
	}
	if (linea_pokemon[i] != ';')
		return false;
	i++;
	while (isdigit(linea_pokemon[i])) {
		i++;
	}
	if (linea_pokemon[i] != ';')
		return false;
	i++;
	while (isdigit(linea_pokemon[i])) {
		i++;
	}
	if (linea_pokemon[i] != '\0')
		return false;
	return true;
}

/*
* recibe un caracter que hace referencia al tipo de pokemon
* retonar al tipo correspondiente del pokemon
*/
tipo_pokemon convertir_tipo(const char tipo)
{
	switch (tipo) {
	case 'A':
		return TIPO_AGUA;
	case 'E':
		return TIPO_ELECTRICO;
	case 'F':
		return TIPO_FUEGO;
	case 'P':
		return TIPO_PLANTA;
	case 'R':
		return TIPO_ROCA;
	case 'N':
		return TIPO_NORMAL;
	case 'L':
		return TIPO_LUCHA;
	default:
		return TIPO_NORMAL;
	}
}

/*
* recibe como parametros a la pokedex y una linea de un archivo
* realiza el parseo de la linea a datos1
* carga los datos a un struct pokemon *
* retorna el struct pokemon * si pudo cargar los datos
* NULL si no lo pudo hacer
*/

struct pokemon *cargar_pokemon(pokedex_t *mi_pokedex, const char *linea_pokemon)
{
	struct pokemon *nuevo_pokemon = malloc(sizeof(struct pokemon));
	if (nuevo_pokemon == NULL) {
		return NULL;
	}
	if (mi_pokedex->array_nombres_pokemones == NULL) {
		mi_pokedex->array_nombres_pokemones = malloc(
			(mi_pokedex->cantidad_pokemones + 1) * sizeof(char *));
		if (mi_pokedex->array_nombres_pokemones == NULL) {
			return NULL;
		}
	} else {
		char **array_nombres_aux = realloc(
			mi_pokedex->array_nombres_pokemones,
			(mi_pokedex->cantidad_pokemones + 1) * sizeof(char *));
		if (array_nombres_aux == NULL) {
			return NULL;
		}
		mi_pokedex->array_nombres_pokemones = array_nombres_aux;
	}

	size_t i = 0;
	size_t j = 0;
	unsigned id;
	char tipo;
	unsigned fuerza;
	unsigned destreza;
	unsigned inteligencia;
	size_t len_nombre_pokemon = 32;
	char *nombre_pokemon = malloc(len_nombre_pokemon * sizeof(char));
	if (nuevo_pokemon == NULL) {
		free(nuevo_pokemon);
		return NULL;
	}
	sscanf(linea_pokemon, "%u;%*[^;];%c;%u;%u;%u", &id, &tipo, &fuerza,
	       &destreza, &inteligencia);
	while (linea_pokemon[i] != ';') {
		i++;
	}
	i++;
	while (linea_pokemon[i] != ';') {
		if (j >= len_nombre_pokemon) {
			len_nombre_pokemon = len_nombre_pokemon * 2;
			char *aux_nombre =
				realloc(nombre_pokemon,
					len_nombre_pokemon * sizeof(char));
			if (aux_nombre == NULL) {
				free(nombre_pokemon);
				free(nuevo_pokemon);
				return NULL;
			}
			nombre_pokemon = aux_nombre;
		}
		nombre_pokemon[j] = linea_pokemon[i];
		i++;
		j++;
	}

	nombre_pokemon[j] = '\0';

	nuevo_pokemon->id = id;
	nuevo_pokemon->nombre = nombre_pokemon;
	nuevo_pokemon->tipo = convertir_tipo(tipo);
	nuevo_pokemon->fuerza = fuerza;
	nuevo_pokemon->destreza = destreza;
	nuevo_pokemon->inteligencia = inteligencia;

	mi_pokedex->array_nombres_pokemones[mi_pokedex->cantidad_pokemones] =
		nombre_pokemon;

	return nuevo_pokemon;
}

/*
* funcion para verificar si hay mas lineas por leer
*/
int archivo_hay_mas_lineas(pokedex_t *pokedex)
{
	int caracter = fgetc(pokedex->archivo);
	if (pokedex == NULL || feof((*pokedex).archivo)) {
		return 0;
	}
	ungetc(caracter, pokedex->archivo);
	return 1;
}

/*
* se espera que lea una linea dinamicamente, si pudo leer retorna la linea si no pudo retorna NULL
*/
char *archivo_leer_linea(pokedex_t *pokedex, char *linea_leida)
{
	if (pokedex == NULL || archivo_hay_mas_lineas(pokedex) == 0) {
		return NULL;
	}
	if (linea_leida != NULL) {
		free(linea_leida);
	}
	int caracter;
	size_t tamanio_inicial = 32;
	size_t longitud = 0;
	char *linea_nueva = malloc(tamanio_inicial * sizeof(char));
	if (linea_nueva == NULL) {
		return NULL;
	}
	caracter = fgetc((*pokedex).archivo);

	while (!feof((*pokedex).archivo) && caracter != '\n') {
		if (longitud >= tamanio_inicial - 1) {
			tamanio_inicial = tamanio_inicial * 2;
			char *temp = realloc(linea_nueva,
					     tamanio_inicial * sizeof(char));
			if (temp == NULL) {
				free(linea_nueva);
				return NULL;
			}
			linea_nueva = temp;
		}
		linea_nueva[longitud] = (char)caracter;
		caracter = fgetc((*pokedex).archivo);
		longitud++;
	}

	linea_nueva[longitud] = '\0';

	return linea_nueva;
}

/*
* se agrega los pokemones a struct pokemon **pokemones_por_id atraves de un algoritmo de insercion simple
*/

void agregar_pokemones_por_id(pokedex_t *pokedex, struct pokemon *nuevo_pokemon)
{
	int i;
	struct pokemon **array_pokemon_aux;
	if (pokedex->cantidad_pokemones == 0) {
		pokedex->pokemones_por_id = malloc(sizeof(struct pokemon *));
		if (pokedex->pokemones_por_id == NULL) {
			return;
		}
		pokedex->pokemones_por_id[0] = nuevo_pokemon;
	} else {
		array_pokemon_aux = realloc(pokedex->pokemones_por_id,
					    (pokedex->cantidad_pokemones + 1) *
						    sizeof(struct pokemon *));
		if (array_pokemon_aux == NULL) {
			return;
		}
		pokedex->pokemones_por_id = array_pokemon_aux;
		i = (int)(pokedex->cantidad_pokemones) - 1;
		bool inserto = false;
		while (i >= 0 && !inserto) {
			if (pokedex->pokemones_por_id[i]->id >
			    nuevo_pokemon->id) {
				pokedex->pokemones_por_id[i + 1] =
					pokedex->pokemones_por_id[i];
				i--;
			} else {
				inserto = true;
			}
		}
		pokedex->pokemones_por_id[i + 1] = nuevo_pokemon;
	}
}

/*
* se agrega los pokemones a struct pokemon **pokemones_por_nombre atraves de un algoritmo de insercion simple
*/
void agregar_pokemones_por_nombre(pokedex_t *pokedex,
				  struct pokemon *nuevo_pokemon)
{
	int i;
	struct pokemon **array_pokemon_aux;
	if (pokedex->cantidad_pokemones == 0) {
		pokedex->pokemones_por_nombre =
			malloc(sizeof(struct pokemon *));
		if (pokedex->pokemones_por_nombre == NULL) {
			return;
		}
		pokedex->pokemones_por_nombre[0] = nuevo_pokemon;
	} else {
		array_pokemon_aux = realloc(pokedex->pokemones_por_nombre,
					    (pokedex->cantidad_pokemones + 1) *
						    sizeof(struct pokemon *));
		if (array_pokemon_aux == NULL) {
			return;
		}
		pokedex->pokemones_por_nombre = array_pokemon_aux;
		i = (int)(pokedex->cantidad_pokemones) - 1;
		bool inserto = false;
		while (i >= 0 && !inserto) {
			if (strcmp(pokedex->pokemones_por_nombre[i]->nombre,
				   nuevo_pokemon->nombre) > 0) {
				pokedex->pokemones_por_nombre[i + 1] =
					pokedex->pokemones_por_nombre[i];
				i--;
			} else {
				inserto = true;
			}
		}

		pokedex->pokemones_por_nombre[i + 1] = nuevo_pokemon;
	}
}

pokedex_t *pokedex_abrir(const char *archivo)
{
	pokedex_t *pokedex;
	pokedex = malloc(sizeof(pokedex_t));
	if (pokedex == NULL) {
		return NULL;
	}

	pokedex->cantidad_pokemones = 0;
	pokedex->array_nombres_pokemones = NULL;
	pokedex->pokemones_por_id = NULL;
	pokedex->pokemones_por_nombre = NULL;

	if (archivo == NULL) {
		free(pokedex);
		return NULL;
	}

	pokedex->archivo = fopen(archivo, "r");
	if (pokedex->archivo == NULL) {
		free(pokedex);
		return NULL;
	}

	char *linea_pokemon = NULL;

	struct pokemon *pokemon_aux;

	bool pokemon_valido = true;
	while (archivo_hay_mas_lineas(pokedex) == 1 && pokemon_valido) {
		linea_pokemon = archivo_leer_linea(pokedex, linea_pokemon);
		if (verificar_pokemon(linea_pokemon)) {
			pokemon_aux = cargar_pokemon(pokedex, linea_pokemon);
		} else {
			pokemon_aux = NULL;
		}
		if (pokemon_aux == NULL) {
			pokemon_valido = false;
		} else {
			agregar_pokemones_por_id(pokedex, pokemon_aux);
			agregar_pokemones_por_nombre(pokedex, pokemon_aux);
			pokedex->cantidad_pokemones++;
		}
	}
	free(linea_pokemon);

	/*
	* ARREGLO CASO BORDE SI LA POKEDEX ESTA VACIA O SI EL PRIMER POKE ES INVALIDO
	*/

	if (pokedex->cantidad_pokemones == 0) {
		fclose(pokedex->archivo);
		free(pokedex);
		return NULL;
	}
	return pokedex;
}

/*
* se busca el pokemon por id mediante busqueda binaria.
*/

const struct pokemon *pokedex_buscar_pokemon_id(pokedex_t *pokedex, unsigned id)
{
	int inicio, medio, fin;

	if (pokedex == NULL) {
		return NULL;
	}
	inicio = 0;
	fin = (int)(pokedex->cantidad_pokemones) - 1;

	while (inicio <= fin) {
		medio = (inicio + fin) / 2;

		if (pokedex->pokemones_por_id[medio]->id == id) {
			return pokedex->pokemones_por_id[medio];
		}

		if (pokedex->pokemones_por_id[medio]->id < id) {
			inicio = medio + 1;
		} else {
			fin = medio - 1;
		}
	}
	/*  
    * si no se encontro ningun pokemon con el nombre
    */
	return NULL;
}

/*
* se busca el pokemon por nombre mediante busqueda binaria.
*/

const struct pokemon *pokedex_buscar_pokemon_nombre(pokedex_t *pokedex,
						    const char *nombre)
{
	int inicio, medio, fin;
	if (pokedex == NULL) {
		return NULL;
	}
	if (nombre == NULL) {
		return NULL;
	}
	inicio = 0;
	fin = (int)(pokedex->cantidad_pokemones) - 1;
	while (inicio <= fin) {
		medio = (inicio + fin) / 2;
		if (strcmp(pokedex->pokemones_por_nombre[medio]->nombre,
			   nombre) == 0) {
			return pokedex->pokemones_por_nombre[medio];
		}

		if (strcmp(pokedex->pokemones_por_nombre[medio]->nombre,
			   nombre) < 0) {
			inicio = medio + 1;
		} else {
			fin = medio - 1;
		}
	}
	/*
    * si no se encontro ningun pokemon con el nombre
    */
	return NULL;
}

void pokedex_destruir(pokedex_t *pokedex)
{
	if (pokedex == NULL) {
		return;
	}
	if (pokedex->archivo != NULL) {
		fclose(pokedex->archivo);
	}
	int i;
	for (i = 0; i < pokedex->cantidad_pokemones; i++) {
		free(pokedex->array_nombres_pokemones[i]);
		free(pokedex->pokemones_por_id[i]);
	}

	free(pokedex->pokemones_por_nombre);
	free(pokedex->pokemones_por_id);
	free(pokedex->array_nombres_pokemones);
	free(pokedex);
}

unsigned pokedex_cantidad_pokemones(pokedex_t *pokedex)
{
	if (pokedex != NULL) {
		return pokedex->cantidad_pokemones;
	}
	return (unsigned)0;
}

/**
 * Invoca la función especificada para cada pokemon en la pokedex.
 * 
 * La función se debe invocar con los pokemon ordenados según el criterio de iteracion dado (creciente por nombre o por id).
 * 
 * La función de iteración recibe un puntero al pokemon y un puntero al contexto y devuelve 
 * true si se debe seguir iterando, o false en caso contrario.
 * 
 * Devuelve la cantidad de pokemones iterados.
 * 
 * Restriccion del TP: implementar con complejidad O(n) en tiempo y O(1) en espacio.
*/

unsigned pokedex_iterar_pokemones(pokedex_t *pokedex, enum modo_iteracion modo,
				  bool (*funcion)(struct pokemon *, void *),
				  void *ctx)
{
	unsigned cantidad_pokemones_iterados = 0;
	int i = 0;
	struct pokemon **pokemones;
	bool seguir_iterando;

	if (pokedex == NULL) {
		return cantidad_pokemones_iterados;
	}
	if (funcion == NULL) {
		return cantidad_pokemones_iterados;
	}
	unsigned cantidad_pokemones_total = pokedex_cantidad_pokemones(pokedex);

	if (modo == ITERAR_NOMBRE) {
		pokemones = pokedex->pokemones_por_nombre;
	} else if (modo == ITERAR_ID) {
		pokemones = pokedex->pokemones_por_id;
	} else {
		return 0;
	}
	seguir_iterando = true;
	while (i < cantidad_pokemones_total && seguir_iterando) {
		seguir_iterando = funcion(pokemones[i], ctx);
		cantidad_pokemones_iterados++;
		i++;
	}
	return cantidad_pokemones_iterados;
}