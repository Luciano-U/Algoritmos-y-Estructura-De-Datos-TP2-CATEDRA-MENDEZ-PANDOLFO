#ifndef MENU_H_
#define MENU_H_

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct operacion operacion_t;

typedef struct menu menu_t;

/**
 * Crea un menú vacío con el nombre especificado.
 *
 * Devuelve un puntero al menú creado o NULL si hubo un error.
 */
menu_t *crear_menu(const char *nombre_del_menu);

/**
 * Crea una operación asociada a una tecla y una función.
 * 
 * La operación representa una acción que se ejecutará al seleccionar la tecla.
 *
 * Devuelve un puntero a la operación creada o NULL si hubo un error.
 */
operacion_t *crear_operacion(const char *nombre_operacion, bool (*f)(void *));

/**
 * Agrega una operación al menú, asociándola a una tecla.
 *
 * Devuelve true si la operación fue agregada correctamente, false en caso contrario.
 */
bool menu_agregar_operacion(menu_t *m, operacion_t *operacion,
			    const char *tecla);

/**
 * Elimina una operación del menú dada la tecla asociada.
 *
 * Devuelve el puntero a la operación eliminada o NULL si no se encontró.
 */
operacion_t *menu_eliminar_operacion(menu_t *m, const char *tecla);

/** 
 * Devuelve la cantidad de operaciones registradas en el menú.
 *
 * Si el menú es NULL, devuelve 0.
 */
size_t cantidad_operaciones(menu_t *m);

/**
 * Inicia el menú y comienza la interacción con el usuario.
 * 
 * Devuelve true si el menú fue iniciado correctamente, false en caso de error.
 */
bool iniciar_menu(menu_t *m, void *contexto);

/**
 * Ejecuta la operación asociada a la tecla proporcionada.
 * 
 * Si la operación no existe o hay un error, devuelve false.
 */
bool ejecutar_operacion(menu_t *m, char *tecla, void *contexto);

/**
 * Muestra todas las operaciones disponibles en el menú.
 */
void mostrar_operaciones(menu_t *m);

/**
 * Destruye la operación y libera la memoria asociada a ella.
 */
void destruir_operacion(void *op);

/**
 * Destruye el menú y libera toda la memoria asociada.
 */
void destruir_menu(menu_t *m);

#endif // MENU_H_
