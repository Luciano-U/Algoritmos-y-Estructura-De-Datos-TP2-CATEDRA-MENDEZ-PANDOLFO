#include "lista.h"
#include <stdio.h>
#include <string.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	size_t cantidad_de_elementos;

	nodo_t *principio;

	nodo_t *final;
};

struct lista_iterador {
	nodo_t *corriente;

	nodo_t *principio;
};

nodo_t *crear_nodo(void *elemento)
{
	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (nuevo_nodo == NULL) {
		return NULL;
	}
	nuevo_nodo->elemento = elemento;
	nuevo_nodo->siguiente = NULL;

	return nuevo_nodo;
}

void destruir_nodo(nodo_t *nodo)
{
	free(nodo);
}

/**
 * Crea una lista vacía.
 *
 */
lista_t *lista_crear()
{
	lista_t *nueva_lista = malloc(sizeof(lista_t));

	if (nueva_lista == NULL) {
		return NULL;
	}
	nueva_lista->principio = NULL;
	nueva_lista->final = NULL;
	nueva_lista->cantidad_de_elementos = 0;

	return nueva_lista;
}
/**
 * Inserta el elemento al final de la lista.
 *
 * Devuelve true si pudo insertar o false si no pudo
 */
bool lista_insertar(lista_t *lista, void *elemento)
{
	nodo_t *nuevo_nodo = crear_nodo(elemento);

	if (nuevo_nodo == NULL) {
		return false;
	}
	if (lista == NULL) {
		free(nuevo_nodo);

		return false;
	}

	if ((lista->cantidad_de_elementos) == 0) {
		lista->principio = nuevo_nodo;
		lista->final = nuevo_nodo;
	} else {
		lista->final->siguiente = nuevo_nodo;
		lista->final = nuevo_nodo;
	}
	(lista->cantidad_de_elementos)++;
	return true;
}
bool lista_insertar_al_principio(lista_t *lista, void *elemento)
{
	nodo_t *nuevo_nodo = crear_nodo(elemento);
	if (nuevo_nodo == NULL) {
		return false;
	}
	if (lista == NULL) {
		free(nuevo_nodo);

		return false;
	}

	if ((lista->cantidad_de_elementos) == 0) {
		lista->principio = nuevo_nodo;
		lista->final = nuevo_nodo;
	} else {
		nuevo_nodo->siguiente = lista->principio;
		lista->principio = nuevo_nodo;
	}
	(lista->cantidad_de_elementos)++;
	return true;
}
/**
 * Inserta un elemento en la posición dada en la lista.
 *
 * Si la posición excede al tamaño de la lista, no se inserta nada.
 *
 * Devuelve true si pudo insertar o false si no pudo
 */
bool lista_insertar_en_posicion(lista_t *lista, int posicion, void *elemento)
{
	nodo_t *nuevo_nodo;
	nodo_t *nodo_aux;
	size_t i;
	if (lista == NULL) {
		return false;
	}
	if (posicion > lista->cantidad_de_elementos || posicion < 0) {
		return false;
	}

	if (posicion == 0) {
		return lista_insertar_al_principio(lista, elemento);
	} else if (lista->cantidad_de_elementos == posicion) {
		return lista_insertar(lista, elemento);
	}

	nuevo_nodo = crear_nodo(elemento);
	if (nuevo_nodo == NULL) {
		return false;
	}
	i = 0;
	nodo_aux = lista->principio;
	while (i < (posicion - 1)) {
		nodo_aux = nodo_aux->siguiente;
		i++;
	}
	nuevo_nodo->siguiente = nodo_aux->siguiente;
	nodo_aux->siguiente = nuevo_nodo;
	lista->cantidad_de_elementos++;
	return true;
}

/**
 * Devuelve la cantidad de elementos presentes en la lista.
 *
 * Devuelve 0 si no existe la lista.
 */
size_t lista_tamanio(lista_t *lista)
{
	if (lista == NULL) {
		return 0;
	}
	return (lista->cantidad_de_elementos);
}

/**
 * Obtiene el elemento en la posición dada o NULL si no existe.
 */
void *lista_obtener_elemento(lista_t *lista, int posicion)
{
	nodo_t *nodo_aux;
	void *elemento;
	if (lista == NULL) {
		return NULL;
	}
	if (posicion >= (lista->cantidad_de_elementos) || posicion < 0) {
		return NULL;
	}
	size_t i = 0;
	nodo_aux = lista->principio;

	while (i < posicion) {
		nodo_aux = nodo_aux->siguiente;
		i++;
	}
	elemento = nodo_aux->elemento;

	return elemento;
}

void *lista_sacar_primero(lista_t *lista)
{
	nodo_t *nodo_a_eliminar;
	void *elemento;
	if (lista == NULL) {
		return NULL;
	}
	nodo_a_eliminar = lista->principio;
	lista->principio = lista->principio->siguiente;
	elemento = nodo_a_eliminar->elemento;
	destruir_nodo(nodo_a_eliminar);
	(lista->cantidad_de_elementos)--;
	return elemento;
}

/**
 * Saca de la lista el elemento en la posición dada y lo devuelve.
 *
 * Si no existe la posición devuelve null.
 */
void *lista_sacar_de_posicion(lista_t *lista, int posicion)
{
	nodo_t *nodo_aux;
	nodo_t *nodo_a_eliminar;
	void *elemento;
	if (lista == NULL) {
		return NULL;
	}
	if ((lista->cantidad_de_elementos) == 0) {
		return NULL;
	}
	if (posicion > (lista->cantidad_de_elementos) || posicion < 0) {
		return NULL;
	}
	if (posicion == 0) {
		return lista_sacar_primero(lista);
	}
	size_t i = 0;
	nodo_aux = lista->principio;

	while (i < posicion - 1) {
		nodo_aux = nodo_aux->siguiente;
		i++;
	}
	nodo_a_eliminar = nodo_aux->siguiente;
	nodo_aux->siguiente = nodo_aux->siguiente->siguiente;
	elemento = nodo_a_eliminar->elemento;
	destruir_nodo(nodo_a_eliminar);
	(lista->cantidad_de_elementos)--;
	return elemento;
}
/**
 * Saca de la lista el elemento pasado por parámetro y lo devuelve.
 *
 * En caso de no existir dicho elemento, devuelve null.
 */
void *lista_sacar_elemento(lista_t *lista, void *elemento)
{
	void *elemento_encontrado;
	nodo_t *nodo_aux;
	nodo_t *nodo_a_eliminar;

	if (lista == NULL || lista_tamanio(lista) == 0) {
		return NULL;
	}

	// Si el primer nodo contiene el elemento
	if (lista->principio->elemento == elemento) {
		return lista_sacar_primero(lista);
	}

	nodo_aux = lista->principio;
	while (nodo_aux->siguiente != NULL &&
	       nodo_aux->siguiente->elemento != elemento) {
		nodo_aux = nodo_aux->siguiente;
	}

	// Si no se encuentra el elemento
	if (nodo_aux->siguiente == NULL) {
		return NULL;
	}

	// Si se encontró el nodo
	nodo_a_eliminar = nodo_aux->siguiente;
	nodo_aux->siguiente = nodo_aux->siguiente->siguiente;
	elemento_encontrado = nodo_a_eliminar->elemento;
	destruir_nodo(nodo_a_eliminar);
	(lista->cantidad_de_elementos)--;

	return elemento_encontrado;
}

/**
 * Devuelve la posición del elemento en la lista. Si no existe el elemento
 * devuelve -1.
 *
 */
int lista_buscar_posicion(lista_t *lista, void *elemento)
{
	nodo_t *nodo_aux;
	int i;
	if (lista == NULL) {
		return -1;
	}
	if (lista->principio == NULL) {
		return -1;
	}
	nodo_aux = lista->principio;
	i = 0;
	while (i < (lista->cantidad_de_elementos) &&
	       nodo_aux->elemento != elemento) {
		nodo_aux = nodo_aux->siguiente;
		i++;
	}
	if (i == (lista->cantidad_de_elementos)) {
		return -1;
	}
	return i;
}

/**
 * Busca en la lista el primer elemento que cumple con la función de criterio.
 * La función recibe como primer parámetro el elemento de la lista y como
 * segundo parámetro el contexto. Si la función devuelve true, el elemento
 * cumple el criterio.
 *
 * Devuelve el primer elemento que cumple con el criterio o NULL si
 * no hay ninguno.
 */
void *lista_buscar(lista_t *lista, bool (*criterio)(void *, void *),
		   void *contexto)
{
	nodo_t *nodo_aux;
	size_t i;
	if (lista == NULL) {
		return NULL;
	}
	if (criterio == NULL) {
		return NULL;
	}
	if (lista->principio == NULL) {
		return NULL;
	}
	i = 0;
	nodo_aux = lista->principio;
	while (i < (lista->cantidad_de_elementos) &&
	       !criterio(nodo_aux->elemento, contexto)) {
		nodo_aux = nodo_aux->siguiente;
		i++;
	}
	if (i == (lista->cantidad_de_elementos)) {
		return NULL;
	}

	return nodo_aux->elemento;
}

/**
 * Itera los elementos de la lista aplicando la función f a cada elemento de la
 * misma. Contexto se pasa como segundo parámetro de f.
 *
 * Deja de iterar si la función f devuelve false.
 *
 * Devuelve la cantidad de veces que fue invocada f.
 */
int lista_iterar(lista_t *lista, bool (*f)(void *, void *), void *contexto)
{
	nodo_t *nodo_aux;
	size_t i = 0;
	int cantidad_iterados = 0;
	bool parar_iteracion;
	if (lista == NULL) {
		return cantidad_iterados;
	}
	if (f == NULL) {
		return cantidad_iterados;
	}
	if (lista->principio == NULL) {
		return cantidad_iterados;
	}
	nodo_aux = lista->principio;
	parar_iteracion = true;
	while (i < (lista->cantidad_de_elementos) && parar_iteracion) {
		if (f(nodo_aux->elemento, contexto) == false) {
			parar_iteracion = false;
		}
		nodo_aux = nodo_aux->siguiente;
		i++;
		cantidad_iterados++;
	}

	return cantidad_iterados;
}
/**
 * Destruye la lista.
 */
void lista_destruir(lista_t *lista)
{
	nodo_t *nodo_a_eliminar;
	size_t i;
	if (lista == NULL) {
		return;
	}

	size_t tamanio_lista = (lista->cantidad_de_elementos);

	for (i = 0; i < tamanio_lista; i++) {
		nodo_a_eliminar = lista->principio;
		lista->principio = lista->principio->siguiente;
		destruir_nodo(nodo_a_eliminar);
	}

	free(lista);
}
/**
 * Crea un iterador para la lista.
 * 
 * Devuelve el iterador o NULL en caso de error.
 */
lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (iterador == NULL) {
		return NULL;
	}
	if (lista == NULL) {
		free(iterador);
		return NULL;
	}
	iterador->corriente = lista->principio;
	iterador->principio = lista->principio;

	return iterador;
}

/**
 * Devuelve true si quedan elementos por recorrer en el iterador.
 * Devuelve false si no quedan elementos por recorrer.
 */
bool lista_iterador_quedan_elementos_por_recorrer(lista_iterador_t *iterador)
{
	if (iterador == NULL) {
		return false;
	}
	if (iterador->corriente != NULL) {
		return true;
	}
	return false;
}
/**
 * Avanza el iterador a la siguiente posición.
 *
 * Si no quedan elementos por recorrer, no hace nada.
 */
void lista_iterador_proxima_iteracion(lista_iterador_t *iterador)
{
	if (lista_iterador_quedan_elementos_por_recorrer(iterador)) {
		iterador->corriente = iterador->corriente->siguiente;
	}
}

/**
 * Devuelve el elemento en la posición actual del iterador.
 *
 * Si no quedan elementos por recorrer, devuelve NULL.
 */
void *lista_iterador_obtener_elemento(lista_iterador_t *iterador)
{
	if (iterador == NULL) {
		return NULL;
	}
	void *elemento;
	if (iterador->corriente == NULL) {
		return NULL;
	}
	elemento = iterador->corriente->elemento;

	return elemento;
}
/**
 * Destruye el iterador.
 *
 * No destruye la lista.
 */
void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (iterador == NULL) {
		free(iterador);
		return;
	}
	free(iterador);
}

void lista_iterador_reiniciar(lista_iterador_t *iterador)
{
	iterador->corriente = iterador->principio;
}