#ifndef JUEGO_H_
#define JUEGO_H_

#include "mapa.h"
#include "mapa_objetos.h"

#define MAX_MAPAS 2
#define MAX_JUGADORES 2

typedef struct juego {
	mapa_t *mapa[MAX_MAPAS];
	jugador_t *jugadores[MAX_JUGADORES];
	lista_t *pokemones_cargados_en_el_juego;
	int semilla;
	unsigned int tiempo_maximo_de_juego;
} juego_t;

/*
    retorna juego_t con una semilla
    retorna NULL SI NO PUDO CREAR AL JUEGO
*/
juego_t *crear_juego(int semilla);
/* 
    Recibe como parametros el juego y una pokedex
    retorna true si pudo cargar los datos correspondientes al juego
    retora false en caso contrario
*/
bool cargar_datos_al_juego(juego_t *j, pokedex_t *p);
/*
    recibe como parametros un mapa y un jugador
    carga los pokesmones almacenados en mapa_t
    - si carga un pokemon en la posicion inicial del jugador lo anade a su lista de capturados
*/
void cargar_pokemones_al_mapa(mapa_t *m, jugador_t *j);
/*
    Retorna true si pudo destruir el juego
    retorna false si no pduo destruir el juego o es null
*/
bool destruir_juego(juego_t *j);

#endif // JUEGO_H_
