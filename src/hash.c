#include "hash.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define FACTOR_DE_CARGAR 0.65f

size_t hash_funcion(const char *string)
{
	size_t hash = 5381;
	size_t c;

	while ((c = (size_t)(*string++))) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

typedef struct tabla_hash_item {
	void *elemento;
	char *clave;
} tabla_hash_item_t;

struct hash {
	float factor_de_carga;
	tabla_hash_item_t **tabla_hash;
	size_t capacidad;
	size_t cantidad;
	size_t (*f)(const char *clave);
};
/**
 * Crea una tabla de hash de direccionamiento abierto con la capacidad inicial
 * dada. Si la capacidad es menor a 3, se utiliza 3 como capacidad inicial.
 *
 * Devuelve la tabla creada o NULL en caso de error.
 */
hash_t *hash_crear(size_t capacidad_inicial)
{
	hash_t *h = malloc(sizeof(hash_t));
	if (h == NULL) {
		return NULL;
	}
	h->cantidad = 0;
	h->f = hash_funcion;
	if (capacidad_inicial < 3) {
		h->capacidad = 3;
	} else {
		h->capacidad = capacidad_inicial;
	}

	h->factor_de_carga = FACTOR_DE_CARGAR;
	h->tabla_hash = malloc(h->capacidad * sizeof(tabla_hash_item_t *));
	if (h->tabla_hash == NULL) {
		free(h);

		return NULL;
	}
	for (size_t i = 0; i < h->capacidad; i++) {
		h->tabla_hash[i] = NULL;
	}
	return h;
}

/**
 * Igual que hash_crear, pero permite al usuario proveer su propia funcion de hash.
 * La función f no puede ser nula.
 */
hash_t *hash_crear_con_funcion(size_t capacidad_inicial,
			       size_t (*f)(const char *))
{
	if (f == NULL) {
		return NULL;
	}
	hash_t *h = malloc(sizeof(hash_t));
	if (h == NULL) {
		return NULL;
	}
	h->cantidad = 0;
	h->f = f;
	if (capacidad_inicial < 3) {
		h->capacidad = 3;
	} else {
		h->capacidad = capacidad_inicial;
	}
	h->factor_de_carga = FACTOR_DE_CARGAR;
	h->tabla_hash = malloc(capacidad_inicial * sizeof(tabla_hash_item_t *));
	if (h->tabla_hash == NULL) {
		free(h);
		return NULL;
	}
	for (size_t i = 0; i < h->capacidad; i++) {
		h->tabla_hash[i] = NULL;
	}
	return h;
}

char *duplicar_clave(const char *clave)
{
	if (clave == NULL) {
		return NULL;
	}

	size_t len = strlen(clave);

	char *copia = malloc((len + 1) * sizeof(char));

	if (copia == NULL) {
		return NULL;
	}
	strcpy(copia, clave);

	return copia;
}

tabla_hash_item_t *crear_elemento(const char *clave, void *valor)
{
	tabla_hash_item_t *nuevo_elemento = malloc(sizeof(tabla_hash_item_t));
	if (nuevo_elemento == NULL) {
		return NULL;
	}

	nuevo_elemento->clave = duplicar_clave(clave);
	nuevo_elemento->elemento = valor;

	return nuevo_elemento;
}

void rehash_v2(hash_t *h)
{
	size_t capacidad_original = h->capacidad;
	h->capacidad = h->capacidad * 2;
	tabla_hash_item_t **nueva_tabla =
		malloc(h->capacidad * sizeof(tabla_hash_item_t *));
	if (nueva_tabla == NULL) {
		return;
	}
	for (size_t i = 0; i < h->capacidad; i++) {
		nueva_tabla[i] = NULL;
	}
	for (size_t i = 0; i < capacidad_original; i++) {
		if (h->tabla_hash[i] != NULL) {
			size_t nuevo_indice =
				h->f(h->tabla_hash[i]->clave) % h->capacidad;
			if (nueva_tabla[nuevo_indice] == NULL) {
				nueva_tabla[nuevo_indice] = h->tabla_hash[i];
			} else {
				size_t siguiente_indice =
					(nuevo_indice + 1) % h->capacidad;
				while (siguiente_indice != nuevo_indice &&
				       nueva_tabla[siguiente_indice] != NULL) {
					siguiente_indice =
						(siguiente_indice + 1) %
						h->capacidad;
				}
				nueva_tabla[siguiente_indice] =
					h->tabla_hash[i];
			}
		}
	}
	free(h->tabla_hash);
	h->tabla_hash = nueva_tabla;
}

/**
 * Inserta una clave en la tabla y asocia el valor pasado por parámetro.
 *
 * Si previamente existe un valor asociado a esa clave y anterior no es nulo, se
 * guarda un puntero al valor anterior en el puntero.
 *
 * Devuelve true si pudo insertar/actualizar el valor.
 */
bool hash_insertar(hash_t *h, const char *clave, void *valor, void **anterior)
{
	if (h == NULL || clave == NULL) {
		return false;
	}

	if (anterior != NULL) {
		*anterior = NULL;
	}
	size_t indice_a_insertar = (h->f(clave) % h->capacidad);
	size_t nuevo_indice;
	bool se_inserto = false;
	if (h->tabla_hash[indice_a_insertar] == NULL) {
		tabla_hash_item_t *elemento_a_insertar =
			crear_elemento(clave, valor);
		h->tabla_hash[indice_a_insertar] = elemento_a_insertar;
		h->cantidad++;
		se_inserto = true;

	} else if (strcmp(h->tabla_hash[indice_a_insertar]->clave, clave) ==
		   0) {
		if (anterior != NULL) {
			*anterior = h->tabla_hash[indice_a_insertar]->elemento;
		}
		h->tabla_hash[indice_a_insertar]->elemento = valor;
		se_inserto = true;
	} else {
		nuevo_indice = (indice_a_insertar + 1) % h->capacidad;

		while (!se_inserto && nuevo_indice != indice_a_insertar) {
			if (h->tabla_hash[nuevo_indice] == NULL) {
				tabla_hash_item_t *elemento_a_insertar =
					crear_elemento(clave, valor);
				h->tabla_hash[nuevo_indice] =
					elemento_a_insertar;
				h->cantidad++;
				se_inserto = true;

			} else if (strcmp(h->tabla_hash[nuevo_indice]->clave,
					  clave) == 0) {
				if (anterior != NULL) {
					*anterior = h->tabla_hash[nuevo_indice]
							    ->elemento;
				}
				h->tabla_hash[nuevo_indice]->elemento = valor;
				se_inserto = true;
			}
			nuevo_indice++;
			nuevo_indice = nuevo_indice % h->capacidad;
		}
	}
	if (((float)h->cantidad / (float)h->capacidad) >= h->factor_de_carga) {
		rehash_v2(h);
		//rehash(h);
	}
	return se_inserto;
}

/**
 * Quita de la tabla una clave y devuelve el valor asociado.
 *
 * Devuelve el valor asociado o NULL en caso de error.
 */

void repocisionar(hash_t *h, size_t indice_vacio)
{
	size_t pos_actual = (indice_vacio + 1) % h->capacidad;

	while (h->tabla_hash[pos_actual] != NULL) {
		size_t indice_ideal =
			h->f(h->tabla_hash[pos_actual]->clave) % h->capacidad;

		if ((indice_ideal <= indice_vacio &&
		     indice_vacio < pos_actual) ||
		    (pos_actual < indice_ideal &&
		     indice_ideal <= indice_vacio) ||
		    (indice_vacio < pos_actual && pos_actual < indice_ideal)) {
			h->tabla_hash[indice_vacio] = h->tabla_hash[pos_actual];
			h->tabla_hash[pos_actual] = NULL;
			indice_vacio = pos_actual;
		}
		pos_actual = (pos_actual + 1) % h->capacidad;
	}
}
void *hash_sacar(hash_t *h, const char *clave)
{
	if (h == NULL) {
		return NULL;
	}
	if (clave == NULL) {
		return NULL;
	}
	size_t indice_a_buscar = h->f(clave) % h->capacidad;
	size_t siguiente_indice;
	bool se_saco = false;
	void *valor_asociado = NULL;
	if (h->tabla_hash[indice_a_buscar] != NULL) {
		if (strcmp(h->tabla_hash[indice_a_buscar]->clave, clave) == 0) {
			valor_asociado =
				h->tabla_hash[indice_a_buscar]->elemento;
			free(h->tabla_hash[indice_a_buscar]->clave);
			free(h->tabla_hash[indice_a_buscar]);
			h->tabla_hash[indice_a_buscar] = NULL;
			if (h->tabla_hash[(indice_a_buscar + 1) % h->capacidad] !=
			    NULL) {
				repocisionar(h, indice_a_buscar);
			}
			h->cantidad--;
		} else {
			siguiente_indice = (indice_a_buscar + 1) % h->capacidad;
			while (siguiente_indice != indice_a_buscar &&
			       !se_saco &&
			       h->tabla_hash[siguiente_indice] != NULL) {
				if (strcmp(h->tabla_hash[siguiente_indice]
						   ->clave,
					   clave) == 0) {
					valor_asociado =
						h->tabla_hash[siguiente_indice]
							->elemento;
					free(h->tabla_hash[siguiente_indice]
						     ->clave);
					free(h->tabla_hash[siguiente_indice]);
					h->tabla_hash[siguiente_indice] = NULL;
					if (h->tabla_hash[siguiente_indice + 1] !=
					    NULL) {
						repocisionar(h,
							     siguiente_indice);
					}
					h->cantidad--;
					se_saco = true;
				}
				siguiente_indice++;
				siguiente_indice =
					siguiente_indice % h->capacidad;
			}
		}
	}

	return valor_asociado;
}

/**
 * Busca en la tabla el valor asociado a una clave. En caso de no existir
 * devuelve NULL.
 */
void *hash_buscar(hash_t *h, const char *clave)
{
	if (h == NULL || clave == NULL) {
		return NULL;
	}
	size_t indice_a_buscar = h->f(clave) % h->capacidad;
	size_t nuevo_indice_a_buscar;
	bool se_encontro = false;
	void *valor_asociado = NULL;
	if (h->tabla_hash[indice_a_buscar] == NULL) {
		valor_asociado = NULL;
	} else {
		if (strcmp(h->tabla_hash[indice_a_buscar]->clave, clave) == 0) {
			valor_asociado =
				h->tabla_hash[indice_a_buscar]->elemento;
			se_encontro = true;
		}
		nuevo_indice_a_buscar = (indice_a_buscar + 1) % h->capacidad;

		while (!se_encontro &&
		       h->tabla_hash[nuevo_indice_a_buscar] != NULL &&
		       nuevo_indice_a_buscar != indice_a_buscar) {
			if (strcmp(h->tabla_hash[nuevo_indice_a_buscar]->clave,
				   clave) == 0) {
				valor_asociado =
					h->tabla_hash[nuevo_indice_a_buscar]
						->elemento;
				se_encontro = true;
			}
			nuevo_indice_a_buscar++;
			nuevo_indice_a_buscar =
				nuevo_indice_a_buscar % h->capacidad;
		}
	}

	return valor_asociado;
}
/**
 * Devuelve si existe una clave en la tabla.
 */
bool hash_existe(hash_t *h, const char *clave)
{
	if (h == NULL) {
		return false;
	}
	if (clave == NULL) {
		return false;
	}
	bool se_encontro = false;
	size_t indice_a_buscar = h->f(clave) % h->capacidad;
	size_t siguiente_indice;

	if (h->tabla_hash[indice_a_buscar] == NULL) {
		se_encontro = false;
	} else if (strcmp(h->tabla_hash[indice_a_buscar]->clave, clave) == 0) {
		se_encontro = true;
	} else {
		siguiente_indice = (indice_a_buscar + 1) % h->capacidad;
		while (!se_encontro &&
		       h->tabla_hash[siguiente_indice] != NULL &&
		       siguiente_indice != indice_a_buscar) {
			if (strcmp(h->tabla_hash[siguiente_indice]->clave,
				   clave) == 0) {
				se_encontro = true;
			}
			siguiente_indice++;
			siguiente_indice = siguiente_indice % h->capacidad;
		}
	}
	return se_encontro;
}

/**
 * Devuelve la cantidad de claves existentes en la tabla.
 */
size_t hash_tamanio(hash_t *h)
{
	if (h == NULL) {
		return 0;
	}
	return h->cantidad;
}
/**
 * Destruye la tabla.
 */
void hash_destruir(hash_t *h)
{
	if (h == NULL) {
		return;
	}
	size_t i;
	for (i = 0; i < h->capacidad; i++) {
		if (h->tabla_hash[i] != NULL) {
			free(h->tabla_hash[i]->clave);
		}
		free(h->tabla_hash[i]);
	}
	free(h->tabla_hash);
	free(h);
}

/**
 * Destruye la tabla y aplica el destructor a cada valor presente en la tabla.
 */
void hash_destruir_todo(hash_t *h, void (*destructor)(void *))
{
	if (h == NULL) {
		return;
	}
	if (destructor == NULL) {
		hash_destruir(h);
		return;
	}
	size_t i;
	for (i = 0; i < h->capacidad; i++) {
		if (h->tabla_hash[i] != NULL) {
			free(h->tabla_hash[i]->clave);
			if (destructor != NULL) {
				destructor(h->tabla_hash[i]->elemento);
			}
		}
		free(h->tabla_hash[i]);
	}
	free(h->tabla_hash);
	free(h);
}

/**
 * Aplica la función f a cada clave de la tabla pasando como segundo parámetro el contexto.
 *
 * Deja de iterar cuando f devuelve false.
 *
 * Devuelve la cantidad de veces que fue invocada f.
 */
size_t hash_iterar_claves(hash_t *h, bool (*f)(const char *, void *), void *ctx)
{
	if (h == NULL) {
		return 0;
	}
	if (f == NULL) {
		return 0;
	}
	bool seguir_iterando = true;
	size_t cantidad_llamadas = 0;
	size_t i = 0;
	while (i < h->capacidad && seguir_iterando) {
		if (h->tabla_hash[i] != NULL) {
			if (f(h->tabla_hash[i]->clave, ctx) == false) {
				seguir_iterando = false;
				cantidad_llamadas++;
			} else {
				cantidad_llamadas++;
			}
		}
		i++;
	}
	return cantidad_llamadas;
}
