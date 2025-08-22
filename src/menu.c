#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "menu.h"
#include "assets.h"
#include "../extra/engine.h"
#include "hash.h"

struct menu {
	const char *nombre_menu;
	hash_t *tabla_operaciones;
};

struct operacion {
	bool (*f)(void *);
	const char *nombre_operacion;
};
/*
 * Crea un menú con la estructura y nombre elegida por el usuario;  

 */
menu_t *crear_menu(const char *nombre_del_menu)
{
	if (nombre_del_menu == NULL) {
		return NULL;
	}
	menu_t *m = malloc(sizeof(menu_t));
	if (m == NULL) {
		return NULL;
	}
	m->nombre_menu = nombre_del_menu;
	size_t capacidad_inicial = 5;
	m->tabla_operaciones = hash_crear(capacidad_inicial);
	return m;
}

operacion_t *crear_operacion(const char *nombre_operacion, bool (*f)(void *))
{
	if (f == NULL || nombre_operacion == NULL) {
		return NULL;
	}
	operacion_t *op = malloc(sizeof(operacion_t));
	if (op == NULL) {
		return NULL;
	}
	op->nombre_operacion = nombre_operacion;
	op->f = f;
	return op;
}

/*
* Agrega una opción al menú. Recibe la operación a agregar y el nombre de la operación.
*/
bool menu_agregar_operacion(menu_t *m, operacion_t *operacion,
			    const char *letra)
{
	if (m == NULL) {
		return false;
	}
	if (operacion == NULL) {
		return false;
	}
	if (letra == NULL) {
		return false;
	}
	hash_insertar(m->tabla_operaciones, letra, operacion, NULL);
	return true;
}
/*
* Elimina una operación del menú.
* Recibe el nombre de la operación a eliminar.
*/
operacion_t *menu_eliminar_operacion(menu_t *m, const char *letra)
{
	if (m == NULL) {
		return NULL;
	}
	if (letra == NULL) {
		return NULL;
	}
	operacion_t *op = hash_sacar(m->tabla_operaciones, letra);

	return op;
}

/*
* Devuelve la cantidad de operaciones almacenadas en el menú hasta el momento.
*/
size_t cantidad_operaciones(menu_t *m)
{
	if (m == NULL) {
		return 0;
	}
	return hash_tamanio(m->tabla_operaciones);
}

/*
 * Inicia el menú.
 */
bool iniciar_menu(menu_t *m, void *contexto)
{
	if (m == NULL) {
		return false;
	}
	char letra[10];
	int leido;
	do {
		mostrar_operaciones(m);
		printf("elegi una: ");
		leido = scanf("%s", letra);
		letra[0] = (char)toupper(letra[0]);
		int c;
		while ((c = getchar()) != '\n' && c != EOF)
			;

		if (leido != 1) {
			printf("Error al leer la entrada.\n");
			return false;
		}

	} while (ejecutar_operacion(m, letra, contexto));
	return true;
}

/*
* Ejecuta una operación basada en la tecla proporcionada.
* El comportamiento de la operación puede depender del contexto actual.
*/
bool ejecutar_operacion(menu_t *m, char *tecla, void *contexto)
{
	if (m == NULL) {
		return false;
	}
	if (tecla == NULL) {
		return false;
	}
	//convertir_a_mayusculas(tecla);
	operacion_t *operacion_a_realizar =
		hash_buscar(m->tabla_operaciones, tecla);
	if (operacion_a_realizar == NULL) {
		return false;
	}
	return operacion_a_realizar->f(contexto);
}

bool mostrar_nombre_operaciones(const char *clave, void *ctx)
{
	if (clave == NULL) {
		return false;
	}
	hash_t *tabla_operaciones = ctx;
	operacion_t *op = hash_buscar(tabla_operaciones, clave);
	printf("%s - %s\n", op->nombre_operacion, clave);
	return true;
}
/*
* Muestra todas las operaciones disponibles en el menú.
*/
void mostrar_operaciones(menu_t *m)
{
	if (m == NULL) {
		return;
	}
	hash_iterar_claves(m->tabla_operaciones, mostrar_nombre_operaciones,
			   m->tabla_operaciones);
}

void destruir_operacion(void *op)
{
	if (op == NULL) {
		return;
	}
	free(op);
}

/*
* Destruye el menú y libera la memoria asociada.
*/
void destruir_menu(menu_t *m)
{
	if (m == NULL) {
		return;
	}
	hash_destruir_todo(m->tabla_operaciones, destruir_operacion);
	free(m);
}
