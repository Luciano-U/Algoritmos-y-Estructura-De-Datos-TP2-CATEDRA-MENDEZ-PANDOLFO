#ifndef MAPA_OBJETOS_H_
#define MAPA_OBJETOS_H_

#include "pila.h"
#include "lista.h"
#include "mapa.h"
#include "pokedex.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct pokemon_juego {
	int x;
	int y;
	bool es_jugador;
	const struct pokemon *p;
} pokemon_juego_t;

typedef struct jugador {
	int x;
	int y;
	bool es_jugador;
	int puntos;
	pila_t *acumulados;
	lista_t *pokes_capturados;
} jugador_t;

/*
    Crea objeto jugador con posicion iniciales (0,0)
    retorna null si no pudo crear al jugador
*/
jugador_t *crear_jugador();

/*
    Recibe como parametros un struct pokemon
    retorna al objeto pokemon_juego_t si pudo crear correctamente al objeto con posicioens x e y aleatorias
    retorna null si no pudo crearlo
*/
pokemon_juego_t *crear_pokemon_juego(const struct pokemon *p);
/*
recibe como parametros un mapa, un jugador, y una nueva posicion x e y
RETORNA true si pudo mover al jugador atraves del mapa
false si excede los limites o la posicion esta siendo ocupada
*/
bool mover_jugador(mapa_t *m, jugador_t *j, int nueva_x, int nueva_y);

/*
Retorna true si pudo destruir al pokemon
false si no pudo destruirlo
*/
bool destruir_pokemon(pokemon_juego_t *p);

/*
Retorna true si pudo destruir al jugador
False si el jugador no existe o no pudo destruirlo
*/

bool destruir_jugador(jugador_t *j);

/* 
OPERACIONES ESPECIFICAS PARA LA LOGICA DEL JUEGO
*/

/*
se agrega un pokemon a la lista de de capturados del jugador 
si la posicion en la que se mueve esta siendo ocupada por un poke. Si lo esta agarra todos los poke y genera mas pokes en el mapa

*/
void *capturar_pokemon(mapa_t *m, jugador_t *j, int x, int y);

/*
obtiene el ultimo pokmeon capturado de la lista del jugador.
Null si esta vacio
*/
void *obtener_ultimo_capturado(jugador_t *j);

size_t cantidad_de_pokemones_capturados(jugador_t *j);
/*
Si el jugador atrapa un pokemon se genera un nuevo pokemon en el mapa.
se pasa como parametros el mapa el jugador y su siguiente posicion (x,y) en donde esta el poke
*/
bool generar_nuevo_pokemon(mapa_t *m, jugador_t *j, int x_sigueinte_del_jugador,
			   int y_siguiente_del_jugador);

/*
	Agrega un pokemon a la pila de los pokemons acumulados del jugador 
	y resta puntos
	*/
void agregar_acumulado(jugador_t *j, pokemon_juego_t *poke);
/*
Verifica si el jugador tiene un pokemon acumulado
*/
bool tiene_acumulado(jugador_t *j);

/*
devuelve el ultimo acumulado del jugador
devuelve null si no hay nada
*/
void *obtener_acumulado(jugador_t *j);

/*
saca el ultimo acumulado del jugador
devuelve null si no hay nada
*/
void *sacar_acumulado(jugador_t *j);

/* 
verifica si el pokemon capturado coincide con el tipo del pokemon del tope de los acumulados
*/
bool verificar_tipo_poke(pokemon_juego_t *poke_1, pokemon_juego_t *poke_2);
/*
	retorna los puntos actuales del jugador
*/
int obtener_puntos_jugador(jugador_t *j);

void restar_puntos(jugador_t *j);

void sumar_puntos(jugador_t *j);

#endif // MAPA_OBJETOS_H_
