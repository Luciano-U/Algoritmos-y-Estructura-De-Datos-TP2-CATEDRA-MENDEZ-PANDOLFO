#include "mapa.h"

mapa_t *crear_mapa()
{
	mapa_t *m = malloc(sizeof(mapa_t));

	for (int i = 0; i < MAX_FILA; i++) {
		for (int j = 0; j < MAX_COLUM; j++) {
			m->mapa[i][j] = lista_crear();
		}
	}
	m->siguiente_pokemon = 0;
	return m;
}

// AGREGAR OBJETO AL MAPA
bool posicion_vacia(mapa_t *m, int x, int y)
{
	if (lista_tamanio(m->mapa[x][y]) == 0) {
		return true;
	}
	return false;
}

bool agregar_objeto_al_mapa(mapa_t *m, int x, int y, void *objeto)
{
	if (x < 0 || x >= MAX_FILA || y < 0 || y >= MAX_COLUM) {
		//fuera de los límites del mapa.
		return false;
	}
	if (lista_tamanio(m->mapa[x][y]) == 0) {
		lista_insertar(m->mapa[x][y], objeto);
	} else {
		lista_insertar(m->mapa[x][y], objeto);
	}
	return true;
}
void *obtener_mapa(mapa_t *m, int x, int y)
{
	if (x < 0 || x >= MAX_FILA || y < 0 || y >= MAX_COLUM) {
		//fuera de los límites del mapa.
		return NULL;
	}
	if (lista_tamanio(m->mapa[x][y]) == 0) {
		return NULL;
	} else {
		return lista_obtener_elemento(m->mapa[x][y], 0);
	}
}
void *sacar_del_mapa(mapa_t *m, int x, int y)
{
	if (x < 0 || x >= MAX_FILA || y < 0 || y >= MAX_COLUM) {
		//fuera de los límites del mapa.
		return NULL;
	}
	if (lista_tamanio(m->mapa[x][y]) == 0) {
		return NULL;
	} else {
		return lista_sacar_de_posicion(m->mapa[x][y], 0);
	}
}

size_t cantidad_de_objetos_en_el_mapa(mapa_t *m)
{
	size_t cantidad_de_objetos = 0;
	for (int i = 0; i < MAX_FILA; i++) {
		for (int j = 0; j < MAX_COLUM; j++) {
			cantidad_de_objetos += lista_tamanio(m->mapa[i][j]);
		}
	}
	return cantidad_de_objetos;
}
void destruir_mapa(mapa_t *m)
{
	if (m == NULL) {
		return;
	}
	for (int i = 0; i < MAX_FILA; i++) {
		for (int j = 0; j < MAX_COLUM; j++) {
			lista_destruir(m->mapa[i][j]);
		}
	}
	free(m);
}