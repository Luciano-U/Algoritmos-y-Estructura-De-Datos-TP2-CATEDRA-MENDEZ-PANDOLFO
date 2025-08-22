#include "pila.h"
#include "lista.h"
#include <stdio.h>

struct pila {
	lista_t *lista;
};

pila_t *pila_crear()
{
	pila_t *pila = malloc(sizeof(pila_t));
	if (pila == NULL) {
		return NULL;
	}
	pila->lista = lista_crear();
	if (pila->lista == NULL) {
		free(pila);
		return NULL;
	}

	return pila;
}
/**
 * Apila un elemento en la pila.
 *
 * Devuelve true si pudo apilar o false si no pudo.
 */
bool pila_apilar(pila_t *pila, void *elemento)
{
	if (pila == NULL) {
		return false;
	}

	lista_insertar_en_posicion(pila->lista, 0, elemento);
	return true;
}
/**
 * Desapila un elemento de la pila y lo devuelve.
 *
 * Devuelve NULL si no hay elementos en la pila.
 */
void *pila_desapilar(pila_t *pila)
{
	void *elemento;
	if (pila == NULL) {
		return NULL;
	}
	elemento = lista_sacar_de_posicion(pila->lista, 0);
	if (elemento == NULL) {
		return NULL;
	}

	return elemento;
}

/**
 * Devuelve true si la pila está vacía.
 *
 * Devuelve false si no lo está.
 */
bool pila_vacia(pila_t *pila)
{
	if (pila == NULL) {
		return true;
	}

	size_t tamanio_pila = lista_tamanio(pila->lista);
	if (tamanio_pila == 0) {
		return true;
	}

	return false;
}

/**
 * Devuelve la cantidad de elementos presentes en la pila.
 *
 * Devuelve 0 si no existe la pila.
 */
size_t pila_tamanio(pila_t *pila)
{
	if (pila == NULL) {
		return 0;
	}
	size_t tamanio_pila = lista_tamanio(pila->lista);

	return tamanio_pila;
}

/**
 * Devuelve el elemento en el tope de la pila sin desapilarlo.
 *
 * Devuelve NULL si no hay elementos en la pila.
 */
void *pila_tope(pila_t *pila)
{
	if (pila == NULL) {
		return NULL;
	}
	if (pila_vacia(pila)) {
		return NULL;
	}
	void *elemento = lista_obtener_elemento(pila->lista, 0);

	return elemento;
}
/**
 * Destruye la pila.
 */
void pila_destruir(pila_t *pila)
{
	if (pila == NULL) {
		return;
	}
	if (pila->lista == NULL) {
		return;
	}
	lista_destruir(pila->lista);
	free(pila);
}
