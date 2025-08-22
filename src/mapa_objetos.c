#include "mapa_objetos.h"
#include <time.h>

jugador_t *crear_jugador()
{
	jugador_t *j = malloc(sizeof(jugador_t));
	if (j == NULL) {
		return NULL;
	}
	j->x = 0;
	j->y = 0;
	j->es_jugador = true;
	j->pokes_capturados = lista_crear();
	j->acumulados = pila_crear();
	j->puntos = 10;
	return j;
}

bool mover_jugador(mapa_t *m, jugador_t *j, int nueva_x, int nueva_y)
{
	// Verificar que las nuevas coordenadas están dentro de los límites del mapa
	if (m == NULL || j == NULL) {
		return false;
	}
	if (j->x == nueva_x && j->y == nueva_y) {
		return true;
	}
	if (nueva_x < 0 || nueva_x >= MAX_FILA || nueva_y < 0 ||
	    nueva_y >= MAX_COLUM) {
		return false;
	}
	if (posicion_vacia(m, nueva_x, nueva_y)) {
		sacar_del_mapa(m, j->x, j->y);
		j->x = nueva_x;
		j->y = nueva_y;
		agregar_objeto_al_mapa(m, j->x, j->y, j);
		return true;
	} else {
		return false;
	}
}

/*
    retorna los puntos actuales del jugador
*/
int obtener_puntos_jugador(jugador_t *j)
{
	if (j == NULL) {
		return -1;
	}
	return j->puntos;
}

pokemon_juego_t *crear_pokemon_juego(const struct pokemon *p)
{
	if (p == NULL) {
		return NULL;
	}
	pokemon_juego_t *poke = malloc(sizeof(pokemon_juego_t));

	poke->x = rand() % MAX_COLUM;
	poke->y = rand() % MAX_FILA;
	poke->p = p;
	poke->es_jugador = false;

	return poke;
}
bool destruir_pokemon(pokemon_juego_t *p)
{
	if (p == NULL) {
		return false;
	}
	free(p); // liberar struct pokemon se encarga la pokedex
	return true;
}
bool destruir_jugador(jugador_t *j)
{
	if (j == NULL) {
		return false;
	}
	lista_destruir(j->pokes_capturados);
	pila_destruir(j->acumulados);
	free(j);
	return true;
}

bool generar_nuevo_pokemon(mapa_t *m, jugador_t *j, int x_sigueinte_del_jugador,
			   int y_siguiente_del_jugador)
{
	if (m == NULL) {
		return false;
	}
	if (m->siguiente_pokemon >= lista_tamanio(m->pokemones_cargados)) {
		m->siguiente_pokemon = 0;
	}
	pokemon_juego_t *poke = lista_obtener_elemento(m->pokemones_cargados,
						       m->siguiente_pokemon);
	m->siguiente_pokemon++;

	while (poke->x == x_sigueinte_del_jugador &&
	       poke->y == y_siguiente_del_jugador) {
		lista_insertar(j->pokes_capturados, poke);
		poke = lista_obtener_elemento(m->pokemones_cargados,
					      m->siguiente_pokemon);
		m->siguiente_pokemon++;
	}
	agregar_objeto_al_mapa(m, poke->x, poke->y, poke);
	return true;
}

void *capturar_pokemon(mapa_t *m, jugador_t *j, int x, int y)
{
	void *poke = sacar_del_mapa(m, x, y);
	lista_insertar(j->pokes_capturados, poke);
	generar_nuevo_pokemon(m, j, x, y);
	return poke;
}

void *obtener_ultimo_capturado(jugador_t *j)
{
	if (j == NULL || lista_tamanio(j->pokes_capturados) == 0) {
		return NULL;
	}

	size_t tamanio = lista_tamanio(j->pokes_capturados);
	int indice = (int)(tamanio - 1);
	return lista_obtener_elemento(j->pokes_capturados, indice);
}
size_t cantidad_de_pokemones_capturados(jugador_t *j)
{
	if (j == NULL) {
		return 0;
	}
	return lista_tamanio(j->pokes_capturados);
}
void agregar_acumulado(jugador_t *j, pokemon_juego_t *poke)
{
	if (j == NULL) {
		return;
	}
	if (poke == NULL) {
		return;
	}
	pila_apilar(j->acumulados, poke);
}

bool tiene_acumulado(jugador_t *j)
{
	if (j == NULL) {
		return false;
	}
	if (pila_vacia(j->acumulados) != true) {
		return true;
	}
	return false;
}

void *obtener_acumulado(jugador_t *j)
{
	if (j == NULL) {
		return NULL;
	}
	return pila_tope(j->acumulados);
}

void *sacar_acumulado(jugador_t *j)
{
	if (j == NULL) {
		return NULL;
	}
	return pila_desapilar(j->acumulados);
}

bool verificar_tipo_poke(pokemon_juego_t *poke_1, pokemon_juego_t *poke_2)
{
	if (poke_1 == NULL || poke_2 == NULL) {
		return false;
	}
	if (poke_1->p->tipo == poke_2->p->tipo) {
		return true;
	}
	return false;
}

void restar_puntos(jugador_t *j)
{
	if (j == NULL) {
		return;
	}
	j->puntos--;
}

void sumar_puntos(jugador_t *j)
{
	if (j == NULL) {
		return;
	}
	j->puntos++;
}