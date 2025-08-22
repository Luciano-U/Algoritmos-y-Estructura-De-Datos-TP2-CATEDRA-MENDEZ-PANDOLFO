#include "juego_datos.h"
#include "mapa_objetos.h"
#define CANT_MAX_POKEMONES 4

juego_t *crear_juego(int semilla)
{
	juego_t *j = malloc(sizeof(juego_t));
	if (j == NULL) {
		return NULL;
	}
	j->mapa[0] = crear_mapa();
	j->mapa[1] = crear_mapa();
	j->jugadores[0] = crear_jugador();
	j->jugadores[1] = crear_jugador();
	j->pokemones_cargados_en_el_juego = lista_crear();
	j->semilla = semilla;
	j->mapa[0]->pokemones_cargados = j->pokemones_cargados_en_el_juego;
	j->mapa[1]->pokemones_cargados = j->pokemones_cargados_en_el_juego;
	j->tiempo_maximo_de_juego = 1 * 60 * 5;
	return j;
}

void cargar_pokemones_al_mapa(mapa_t *m, jugador_t *j)
{
	int i = 0;
	while (i < CANT_MAX_POKEMONES) {
		pokemon_juego_t *poke = lista_obtener_elemento(
			m->pokemones_cargados, m->siguiente_pokemon);
		if (m->siguiente_pokemon >=
		    lista_tamanio(m->pokemones_cargados)) {
			m->siguiente_pokemon = 0;
		}
		if (poke->x == j->x && poke->y == j->y) {
			lista_insertar(j->pokes_capturados, poke);
			i--;
		} else {
			agregar_objeto_al_mapa(m, poke->x, poke->y, poke);
		}
		m->siguiente_pokemon++;
		i++;
	}
}

bool cargar_pokemones_al_juego(struct pokemon *poke, void *ctx)
{
	if (poke == NULL) {
		return false;
	}
	juego_t *j = ctx;
	const struct pokemon *poke_a_insertar = poke;
	pokemon_juego_t *poke_juego = crear_pokemon_juego(poke_a_insertar);
	lista_insertar(j->pokemones_cargados_en_el_juego, poke_juego);
	return true;
}

/*
    DESORDENAMOS LA LISTA DE POKEMONES QUE VIENEN ORDENADOS POR ID O POR NOMBRE
*/
void desordenar_lista(lista_t *l)
{
	void **vector_elementos = malloc(lista_tamanio(l) * sizeof(void *));
	if (vector_elementos == NULL) {
		return;
	}
	size_t tamanio_inicial = lista_tamanio(l);
	for (size_t i = 0; i < tamanio_inicial; i++) {
		void *elemento = lista_sacar_de_posicion(l, 0);
		vector_elementos[i] = elemento;
	}
	for (size_t i = 0; i < tamanio_inicial; i++) {
		size_t j = (size_t)(rand() % (int)tamanio_inicial);

		void *temp = vector_elementos[i];
		vector_elementos[i] = vector_elementos[j];
		vector_elementos[j] = temp;
	}
	for (size_t i = 0; i < tamanio_inicial; i++) {
		lista_insertar(l, vector_elementos[i]);
	}
	free(vector_elementos);
}

bool cargar_datos_al_juego(juego_t *j, pokedex_t *p)
{
	if (j == NULL) {
		return false;
	}
	srand((unsigned int)j->semilla);

	//agregamos a los jugadores al mapa
	agregar_objeto_al_mapa(j->mapa[0], j->jugadores[0]->x,
			       j->jugadores[0]->y, j->jugadores[0]);
	agregar_objeto_al_mapa(j->mapa[1], j->jugadores[1]->x,
			       j->jugadores[1]->y, j->jugadores[1]);

	for (int i = 0; i < 10; i++) {
		pokedex_iterar_pokemones(p, ITERAR_NOMBRE,
					 cargar_pokemones_al_juego, j);
	}
	desordenar_lista(j->pokemones_cargados_en_el_juego);

	cargar_pokemones_al_mapa(j->mapa[0], j->jugadores[0]);
	cargar_pokemones_al_mapa(j->mapa[1], j->jugadores[1]);

	return true;
}

/*
ITERADOR PARA DESTRUIR LOS POKEMONES
*/
bool iterador_destruir_pokemos_cargados(void *elemento, void *ctx)
{
	if (elemento == NULL) {
		return false;
	}
	pokemon_juego_t *poke = elemento;
	destruir_pokemon(poke);
	return true;
}

bool destruir_juego(juego_t *j)
{
	if (j == NULL) {
		return false;
	}
	for (int i = 0; i < MAX_JUGADORES; i++) {
		destruir_jugador(j->jugadores[i]);
	}
	for (int i = 0; i < MAX_MAPAS; i++) {
		destruir_mapa(j->mapa[i]);
	}

	lista_iterar(j->pokemones_cargados_en_el_juego,
		     iterador_destruir_pokemos_cargados, NULL);
	lista_destruir(j->pokemones_cargados_en_el_juego);

	free(j);
	return true;
}
