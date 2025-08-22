#ifndef MAPA_H_
#define MAPA_H_

#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"
#define MAX_FILA 12
#define MAX_COLUM 12

typedef struct mapa {
	lista_t *mapa[MAX_FILA][MAX_COLUM];
	lista_t *pokemones_cargados;
	int siguiente_pokemon;
} mapa_t;

///// MAPA OPERACIONES

/*
    Crea un MAPA DE 12x12
    null si no se pudo crear el mapa
*/
mapa_t *crear_mapa();
/*
    agrega un objeto en la posicion indica
    retorna true si se pude agregar un objeto
    false si no pudo agregarse el objeto
*/
bool agregar_objeto_al_mapa(mapa_t *m, int x, int y, void *objeto);
/*
    devuelve true si la posicion esta vacia
    false si la posicion esta ocupada
*/
bool posicion_vacia(mapa_t *m, int x, int y);

/*
    Obtiene el objeto del mapa en la posicion indicada sin sacarlo del mapa
    NULL si el lugar esta vacio o si excede los limites del mapa
*/
void *obtener_mapa(mapa_t *m, int x, int y);

/*
    Saca el objeto del mapa en las coordenadas indicadas
    retorna null si el lugar esta vacio o si excede los limites del mapa
*/
void *sacar_del_mapa(mapa_t *m, int x, int y);

size_t cantidad_de_objetos_en_el_mapa(mapa_t *m);
/*
  Destruye el mapa generado
 */
void destruir_mapa(mapa_t *m);
#endif // MAPA_H_
