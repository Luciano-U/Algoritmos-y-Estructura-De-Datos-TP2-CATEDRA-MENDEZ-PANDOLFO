#include "pa2m.h"
#include "src/mapa.h"
#include "src/mapa_objetos.h"
#include "src/juego_datos.h"
#include "src/menu.h"
#include <string.h>
#include <time.h>

typedef struct item_mapa {
	char nombre_item[40];
	int x;
	int y;
} item_mapa_t;
void creacion_y_destrucion_de_mapa()
{
	mapa_t *m = crear_mapa();
	pa2m_afirmar(m != NULL, "se creo correctamente el mapa");
	destruir_mapa(m);
	pa2m_afirmar(true, "se destruyo correctamente el mapa");
}
void pruebas_de_insertar_objetos_en_mapa()
{
	mapa_t *m = crear_mapa();
	pa2m_afirmar(m != NULL, "se creo correctamente el mapa");

	item_mapa_t item_1 = { .nombre_item = "Espada", .x = 3, .y = 1 };
	item_mapa_t item_2 = { .nombre_item = "Escudo", .x = 6, .y = 6 };
	item_mapa_t item_3 = { .nombre_item = "Casco", .x = 10, .y = 8 };
	item_mapa_t item_4 = { .nombre_item = "Pechera", .x = 2, .y = 9 };
	item_mapa_t item_5 = { .nombre_item = "Botas", .x = 11, .y = 0 };

	pa2m_afirmar(agregar_objeto_al_mapa(m, item_1.x, item_1.y, &item_1),
		     "se agrego un objeto al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, item_2.x, item_2.y, &item_2),
		     "se agrego un objeto al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, item_3.x, item_3.y, &item_3),
		     "se agrego un objeto al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, item_4.x, item_4.y, &item_4),
		     "se agrego un objeto al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, item_5.x, item_5.y, &item_5),
		     "se agrego un objeto al mapa");

	pa2m_afirmar(cantidad_de_objetos_en_el_mapa(m) == 5,
		     " el mapa actualmente tiene 5 objetos");

	destruir_mapa(m);

	m = crear_mapa();

	pa2m_afirmar(m != NULL, "se creo un nuevo mapa");

	item_mapa_t item_6 = { .nombre_item = "Libro", .x = 2, .y = 2 };

	for (int i = 0; i < 20; i++) {
		pa2m_afirmar(
			agregar_objeto_al_mapa(m, item_6.x, item_6.y, &item_6),
			"Es posible agregar el mismo objeto en una misma posicion");
	}
	pa2m_afirmar(cantidad_de_objetos_en_el_mapa(m) == 20,
		     "el mapa tiene 20 objetos");

	item_6.x = 7;
	item_6.y = 5;
	pa2m_afirmar(agregar_objeto_al_mapa(m, item_6.x, item_6.y, &item_6),
		     "Es posible agregar un mismo objet en el mapa");
	item_6.x = 2;
	item_6.y = 7;
	pa2m_afirmar(agregar_objeto_al_mapa(m, item_6.x, item_6.y, &item_6),
		     "Es posible agregar un mismo objet en el mapa");
	item_6.x = 1;
	item_6.y = 1;
	pa2m_afirmar(agregar_objeto_al_mapa(m, item_6.x, item_6.y, &item_6),
		     "Es posible agregar un mismo objet en el mapa");
	item_6.x = 9;
	item_6.y = 4;
	pa2m_afirmar(agregar_objeto_al_mapa(m, item_6.x, item_6.y, &item_6),
		     "Es posible agregar un mismo objet en el mapa");

	item_6.x = -2;
	item_6.y = -1;
	pa2m_afirmar(
		agregar_objeto_al_mapa(m, item_6.x, item_6.y, &item_6) == false,
		"No es posible agregar un objeto fuera de los limites del mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(NULL, item_6.x, item_6.y,
					    &item_6) == false,
		     "No es posible agregar");
	destruir_mapa(m);
}

void pruebas_de_obtener_y_extracion_objetos_en_mapa()
{
	mapa_t *m = crear_mapa();
	pa2m_afirmar(m != NULL, " se creo un mapa");

	item_mapa_t item_1 = { .nombre_item = "Pocion", .x = 3, .y = 1 };
	item_mapa_t item_2 = { .nombre_item = "Arco", .x = 6, .y = 6 };
	item_mapa_t item_3 = { .nombre_item = "Baston", .x = 10, .y = 8 };
	item_mapa_t item_4 = { .nombre_item = "Daga", .x = 2, .y = 9 };
	item_mapa_t item_5 = { .nombre_item = "Hechizo", .x = 11, .y = 0 };

	pa2m_afirmar(agregar_objeto_al_mapa(m, item_1.x, item_1.y, &item_1),
		     "se inserto un objeto");
	pa2m_afirmar(agregar_objeto_al_mapa(m, item_2.x, item_2.y, &item_2),
		     "se inserto un objeto");
	pa2m_afirmar(agregar_objeto_al_mapa(m, item_3.x, item_3.y, &item_3),
		     "se inserto un objeto");
	pa2m_afirmar(agregar_objeto_al_mapa(m, item_4.x, item_4.y, &item_4),
		     "se inserto un objeto");
	pa2m_afirmar(agregar_objeto_al_mapa(m, item_5.x, item_5.y, &item_5),
		     "se inserto un objeto");
	pa2m_afirmar(cantidad_de_objetos_en_el_mapa(m) == 5,
		     "la cantidad de objetos en el mapa es 5");

	item_mapa_t *item = obtener_mapa(m, -2, -3);
	pa2m_afirmar(
		item == NULL,
		"Obtener un objeto del mapa en una posicion invalidad devuelve NULL");
	item = obtener_mapa(m, 0, 0);
	pa2m_afirmar(
		item == NULL,
		"Obtener un objeto del mapa en una casilla vacia devuelve NULL");

	item = obtener_mapa(m, 3, 1);
	pa2m_afirmar(
		item != NULL,
		"Obtener un objeto del mapa en una casilla no vacia devuelve un objeto");
	pa2m_afirmar(strcmp(item->nombre_item, item_1.nombre_item) == 0,
		     "se obtuvo el item esperado");

	item = obtener_mapa(m, 6, 6);
	pa2m_afirmar(
		item != NULL,
		"Obtener un objeto del mapa en una casilla no vacia devuelve un objeto");
	pa2m_afirmar(strcmp(item->nombre_item, item_2.nombre_item) == 0,
		     "se obtuvo el item esperado");

	item = obtener_mapa(m, 10, 8);
	pa2m_afirmar(
		item != NULL,
		"Obtener un objeto del mapa en una casilla no vacia devuelve un objeto");
	pa2m_afirmar(strcmp(item->nombre_item, item_3.nombre_item) == 0,
		     "se obtuvo el item esperado");

	item = obtener_mapa(m, 2, 9);
	pa2m_afirmar(
		item != NULL,
		"Obtener un objeto del mapa en una casilla no vacia devuelve un objeto");
	pa2m_afirmar(strcmp(item->nombre_item, item_4.nombre_item) == 0,
		     "se obtuvo el item esperado");

	item = obtener_mapa(m, 11, 0);
	pa2m_afirmar(
		item != NULL,
		"Obtener un objeto del mapa en una casilla no vacia devuelve un objeto");
	pa2m_afirmar(strcmp(item->nombre_item, item_5.nombre_item) == 0,
		     "se obtuvo el item esperado");

	item_mapa_t *extraido = sacar_del_mapa(m, -3, -4);
	pa2m_afirmar(
		extraido == NULL,
		"sacar un objeto del mapa en una posicion invalida devuelve NULL");
	extraido = sacar_del_mapa(m, 1, 0);
	pa2m_afirmar(
		extraido == NULL,
		"sacar un objeto del mapa en una casilla vacia devuelve NULL");

	extraido = sacar_del_mapa(m, 3, 1);
	pa2m_afirmar(extraido != NULL,
		     "se saco un objeto del mapa correctamente");
	pa2m_afirmar(cantidad_de_objetos_en_el_mapa(m) == 4,
		     "se redujo la cantidad de objetos en el mapa");
	pa2m_afirmar(strcmp(extraido->nombre_item, item_1.nombre_item) == 0,
		     "se saco el elemento esperado");

	extraido = sacar_del_mapa(m, 6, 6);
	pa2m_afirmar(extraido != NULL,
		     "se saco un objeto del mapa correctamente");
	pa2m_afirmar(cantidad_de_objetos_en_el_mapa(m) == 3,
		     "se redujo la cantidad de objetos en el mapa");
	pa2m_afirmar(strcmp(extraido->nombre_item, item_2.nombre_item) == 0,
		     "se saco el elemento esperado");

	extraido = sacar_del_mapa(m, 10, 8);
	pa2m_afirmar(extraido != NULL,
		     "se saco un objeto del mapa correctamente");
	pa2m_afirmar(cantidad_de_objetos_en_el_mapa(m) == 2,
		     "se redujo la cantidad de objetos en el mapa");
	pa2m_afirmar(strcmp(extraido->nombre_item, item_3.nombre_item) == 0,
		     "se saco el elemento esperado");

	extraido = sacar_del_mapa(m, 2, 9);
	pa2m_afirmar(extraido != NULL,
		     "se saco un objeto del mapa correctamente");
	pa2m_afirmar(cantidad_de_objetos_en_el_mapa(m) == 1,
		     "se redujo la cantidad de objetos en el mapa");
	pa2m_afirmar(strcmp(extraido->nombre_item, item_4.nombre_item) == 0,
		     "se saco el elemento esperado");

	extraido = sacar_del_mapa(m, 11, 0);
	pa2m_afirmar(extraido != NULL,
		     "se saco un objeto del mapa correctamente");
	pa2m_afirmar(cantidad_de_objetos_en_el_mapa(m) == 0,
		     "se redujo la cantidad de objetos en el mapa");
	pa2m_afirmar(strcmp(extraido->nombre_item, item_5.nombre_item) == 0,
		     "se saco el elemento esperado");

	destruir_mapa(m);
}

void pruebas_de_creacion_y_destrucion_de_jugador()
{
	mapa_t *m = crear_mapa();
	jugador_t *j = crear_jugador();

	pa2m_afirmar(m != NULL, "se creo un mapa correctamente");
	pa2m_afirmar(j != NULL, "se creo un jugador correctamente");

	pa2m_afirmar(obtener_puntos_jugador(j) == 10,
		     "el jugador tiene 10 puntos al crearse");
	pa2m_afirmar(
		obtener_ultimo_capturado(j) == NULL,
		"el jugador empieza con su lista vacia de pokemones capturados");
	pa2m_afirmar(
		obtener_acumulado(j) == NULL,
		"el jugador empieza con su pila de acumulados vacia de pokemones capturados");

	pa2m_afirmar(destruir_jugador(j),
		     "se destruyo al jugador correctamente");
	destruir_mapa(m);
}

void pruebas_de_movimiento_de_jugador()
{
	jugador_t *j = crear_jugador();
	mapa_t *m = crear_mapa();
	pa2m_afirmar(m != NULL, "se creo un mapa");
	pa2m_afirmar(j != NULL, "se creo un jugador");

	pa2m_afirmar(agregar_objeto_al_mapa(m, j->x, j->y, j),
		     "se agrego correctamente el jugador al mapa");

	for (int i = 0; i < MAX_FILA; i++) {
		for (int k = 0; k < MAX_COLUM; k++) {
			pa2m_afirmar(mover_jugador(m, j, i, k),
				     "El jugador puede recorrer el mapa");
		}
	}
	pa2m_afirmar(
		posicion_vacia(m, 0, 0),
		"el jugador se mueven sin dejar rastro de su posicion anterior");

	pa2m_afirmar(posicion_vacia(m, 11, 11) == false,
		     "el jugador recorrio todo el mapa");

	for (int i = MAX_FILA - 1; i >= 0; i--) {
		for (int k = MAX_COLUM - 1; k >= 0; k--) {
			pa2m_afirmar(mover_jugador(m, j, i, k),
				     "El jugador recorre el mapa nuevamente");
		}
	}
	pa2m_afirmar(posicion_vacia(m, 0, 0) == false,
		     "el jugador volvio a su posicion inicial");
	pa2m_afirmar(posicion_vacia(m, 11, 11),
		     "el jugador recorrio correctamente el mapa");

	item_mapa_t item_1 = { .nombre_item = "Manzana", .x = 5, .y = 6 };

	agregar_objeto_al_mapa(m, 5, 6, &item_1);

	pa2m_afirmar(
		mover_jugador(m, j, 5, 6) == false,
		"no es posible mover un jugador en una posicion que esta siendo ocupada");

	pa2m_afirmar(
		(mover_jugador(m, j, -3, 2) == false),
		"no es posible mover al jugador fuera de lso limites del mapa");

	pa2m_afirmar((mover_jugador(NULL, j, 3, 2)) == false,
		     "no es posible mover al jugador en un mapa NULL");

	pa2m_afirmar((mover_jugador(m, NULL, 4, 2)) == false,
		     "no es posible mover un jugador NULL");

	pa2m_afirmar(destruir_jugador(j),
		     "se destruyo al jugador correctamente");

	destruir_mapa(m);
}

void pruebas_de_crecion_y_destrucion_de_pokemones()
{
	const struct pokemon pokemones[10] = {
		{ 1, "Squirtle", TIPO_AGUA, 48, 65, 50 },
		{ 2, "Charmander", TIPO_FUEGO, 52, 60, 50 },
		{ 3, "Bulbasaur", TIPO_PLANTA, 49, 45, 65 },
		{ 4, "Geodude", TIPO_ROCA, 80, 20, 30 },
		{ 5, "Pikachu", TIPO_ELECTRICO, 55, 90, 50 },
		{ 6, "Eevee", TIPO_NORMAL, 55, 55, 45 },
		{ 7, "Machop", TIPO_LUCHA, 80, 35, 35 },
		{ 8, "Growlithe", TIPO_FUEGO, 70, 60, 50 },
		{ 9, "Poliwag", TIPO_AGUA, 50, 90, 40 },
		{ 10, "Oddish", TIPO_PLANTA, 50, 30, 75 }
	};
	pokemon_juego_t *poke_1 = crear_pokemon_juego(&pokemones[0]);
	pokemon_juego_t *poke_2 = crear_pokemon_juego(&pokemones[1]);
	pokemon_juego_t *poke_3 = crear_pokemon_juego(&pokemones[2]);
	pokemon_juego_t *poke_4 = crear_pokemon_juego(&pokemones[3]);
	pokemon_juego_t *poke_5 = crear_pokemon_juego(&pokemones[4]);
	pokemon_juego_t *poke_6 = crear_pokemon_juego(&pokemones[5]);
	pokemon_juego_t *poke_7 = crear_pokemon_juego(&pokemones[6]);
	pokemon_juego_t *poke_8 = crear_pokemon_juego(&pokemones[7]);
	pokemon_juego_t *poke_9 = crear_pokemon_juego(&pokemones[8]);
	pokemon_juego_t *poke_10 = crear_pokemon_juego(&pokemones[9]);
	pokemon_juego_t *poke_11 = crear_pokemon_juego(NULL);

	pa2m_afirmar(poke_1 != NULL, "se creo un pokemon correctamente");
	pa2m_afirmar(poke_2 != NULL, "se creo un pokemon correctamente");
	pa2m_afirmar(poke_3 != NULL, "se creo un pokemon correctamente");
	pa2m_afirmar(poke_4 != NULL, "se creo un pokemon correctamente");
	pa2m_afirmar(poke_5 != NULL, "se creo un pokemon correctamente");
	pa2m_afirmar(poke_6 != NULL, "se creo un pokemon correctamente");
	pa2m_afirmar(poke_7 != NULL, "se creo un pokemon correctamente");
	pa2m_afirmar(poke_8 != NULL, "se creo un pokemon correctamente");
	pa2m_afirmar(poke_9 != NULL, "se creo un pokemon correctamente");
	pa2m_afirmar(poke_10 != NULL, "se creo un pokemon correctamente");

	pa2m_afirmar(poke_11 == NULL,
		     "crear un pokemon con un datos nulos devuelve null");

	pa2m_afirmar(destruir_pokemon(poke_1),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_2),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_3),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_4),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_5),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_6),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_7),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_8),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_9),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_10),
		     "Se destruyo correctamente un pokemon");

	pa2m_afirmar(destruir_pokemon(NULL) == false,
		     "no es posible destruir un pokemon null");
}

void pruebas_agregar_pokemones_al_mapa()
{
	mapa_t *m = crear_mapa();

	const struct pokemon pokemones[10] = {
		{ 1, "Squirtle", TIPO_AGUA, 48, 65, 50 },
		{ 2, "Charmander", TIPO_FUEGO, 52, 60, 50 },
		{ 3, "Bulbasaur", TIPO_PLANTA, 49, 45, 65 },
		{ 4, "Geodude", TIPO_ROCA, 80, 20, 30 },
		{ 5, "Pikachu", TIPO_ELECTRICO, 55, 90, 50 },
		{ 6, "Eevee", TIPO_NORMAL, 55, 55, 45 },
		{ 7, "Machop", TIPO_LUCHA, 80, 35, 35 },
		{ 8, "Growlithe", TIPO_FUEGO, 70, 60, 50 },
		{ 9, "Poliwag", TIPO_AGUA, 50, 90, 40 },
		{ 10, "Oddish", TIPO_PLANTA, 50, 30, 75 }
	};
	pokemon_juego_t *poke_1 = crear_pokemon_juego(&pokemones[0]);
	pokemon_juego_t *poke_2 = crear_pokemon_juego(&pokemones[1]);
	pokemon_juego_t *poke_3 = crear_pokemon_juego(&pokemones[2]);
	pokemon_juego_t *poke_4 = crear_pokemon_juego(&pokemones[3]);
	pokemon_juego_t *poke_5 = crear_pokemon_juego(&pokemones[4]);
	pokemon_juego_t *poke_6 = crear_pokemon_juego(&pokemones[5]);
	pokemon_juego_t *poke_7 = crear_pokemon_juego(&pokemones[6]);
	pokemon_juego_t *poke_8 = crear_pokemon_juego(&pokemones[7]);
	pokemon_juego_t *poke_9 = crear_pokemon_juego(&pokemones[8]);
	pokemon_juego_t *poke_10 = crear_pokemon_juego(&pokemones[9]);

	pa2m_afirmar(agregar_objeto_al_mapa(m, poke_1->x, poke_1->y, poke_1),
		     "se agrego un pokemon al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, poke_2->x, poke_2->y, poke_2),
		     "se agrego un pokemon al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, poke_3->x, poke_3->y, poke_3),
		     "se agrego un pokemon al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, poke_4->x, poke_4->y, poke_4),
		     "se agrego un pokemon al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, poke_5->x, poke_5->y, poke_5),
		     "se agrego un pokemon al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, poke_6->x, poke_6->y, poke_6),
		     "se agrego un pokemon al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, poke_7->x, poke_7->y, poke_7),
		     "se agrego un pokemon al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, poke_8->x, poke_8->y, poke_8),
		     "se agrego un pokemon al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, poke_9->x, poke_9->y, poke_9),
		     "se agrego un pokemon al mapa");
	pa2m_afirmar(agregar_objeto_al_mapa(m, poke_10->x, poke_10->y, poke_10),
		     "se agrego un pokemon al mapa");

	pa2m_afirmar(destruir_pokemon(poke_1),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_2),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_3),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_4),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_5),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_6),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_7),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_8),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_9),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_10),
		     "Se destruyo correctamente un pokemon");
}

void prueba_operaciones_de_jugador()
{
	const struct pokemon pokemones[3] = {
		{ 1, "Squirtle", TIPO_AGUA, 48, 65, 50 },
		{ 2, "Charmander", TIPO_FUEGO, 52, 60, 50 },
		{ 3, "Bulbasaur", TIPO_PLANTA, 49, 45, 65 }

	};

	pokemon_juego_t *poke_1 = crear_pokemon_juego(&pokemones[0]);
	pokemon_juego_t *poke_2 = crear_pokemon_juego(&pokemones[1]);
	pokemon_juego_t *poke_3 = crear_pokemon_juego(&pokemones[2]);
	jugador_t *j = crear_jugador();
	pa2m_afirmar(j != NULL, "se creo un jugador");

	pa2m_afirmar(obtener_puntos_jugador(j) == 10,
		     "inicialmente el jugador tiene 10 puntos");

	restar_puntos(j);
	restar_puntos(j);
	restar_puntos(j);
	restar_puntos(j);
	restar_puntos(j);

	pa2m_afirmar(obtener_puntos_jugador(j) == 5,
		     "se restaron correctamente puntos al jugador");

	sumar_puntos(j);
	sumar_puntos(j);
	sumar_puntos(j);
	sumar_puntos(j);
	sumar_puntos(j);

	pa2m_afirmar(obtener_puntos_jugador(j) == 10,
		     "se aumentaron correctamente puntos al jugador");

	agregar_acumulado(j, poke_1);
	agregar_acumulado(j, poke_2);
	agregar_acumulado(j, poke_3);

	pa2m_afirmar(
		tiene_acumulado(j),
		"se apilaron pokemones a la pila de acumulados del jugador");

	pokemon_juego_t *poke_desapilado = sacar_acumulado(j);
	pa2m_afirmar(poke_desapilado->p->id == 3,
		     "se saco correctamente el ultimo acumulado");

	poke_desapilado = sacar_acumulado(j);
	pa2m_afirmar(poke_desapilado->p->id == 2,
		     "se saco correctamente el ultimo acumulado");
	poke_desapilado = sacar_acumulado(j);
	pa2m_afirmar(poke_desapilado->p->id == 1,
		     "se saco correctamente el ultimo acumulado");

	pa2m_afirmar(destruir_jugador(j),
		     "se destruyo correctamente al jugador");
	pa2m_afirmar(destruir_pokemon(poke_1),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_2),
		     "Se destruyo correctamente un pokemon");
	pa2m_afirmar(destruir_pokemon(poke_3),
		     "Se destruyo correctamente un pokemon");
}

void pruebas_de_creacion_y_destrucion_de_los_datos_del_juego()
{
	juego_t *juego = crear_juego(432);

	pa2m_afirmar(
		juego != NULL,
		"se creo el juego correctamente con una semilla aleatoria");
	pa2m_afirmar(juego->mapa[0] != NULL,
		     "se creo el mapa del jugador 1 correctamente");
	pa2m_afirmar(juego->mapa[1] != NULL,
		     "se creo el mapa del jugador 2 correctamente");
	pa2m_afirmar(juego->jugadores[0] != NULL,
		     "se creo al jugador 1 correctamente");
	pa2m_afirmar(juego->jugadores[1] != NULL,
		     "se creo al jugador 2 correctamente");
	pa2m_afirmar(juego->pokemones_cargados_en_el_juego != NULL,
		     "se creo una lista  vacia de pokemones para el juego");
	pa2m_afirmar(juego->tiempo_maximo_de_juego == 300,
		     "el tiempo del juego es correcto");

	pa2m_afirmar(destruir_juego(juego),
		     "se destruyo correctamente el juego");
}

void prueba_de_cargar_pokemones_al_juego()
{
	juego_t *juego = crear_juego(616);
	pokedex_t *p = pokedex_abrir("datos/pokedex.csv");

	pa2m_afirmar(cargar_datos_al_juego(juego, p),
		     "se cargo correctamente los pokemones al juego");

	for (int i = 0; i < MAX_FILA; i++) {
		for (int j = 0; j < MAX_COLUM; j++) {
			if (posicion_vacia(juego->mapa[0], i, j) == false) {
				pa2m_afirmar(
					posicion_vacia(juego->mapa[0], i, j) ==
						false,
					"la posicion esta siendo ocupada en (%i,%i) en el mapa del jugador 1",
					i, j);
			}
		}
	}
	for (int i = 0; i < MAX_FILA; i++) {
		for (int j = 0; j < MAX_COLUM; j++) {
			if (posicion_vacia(juego->mapa[1], i, j) == false) {
				pa2m_afirmar(
					posicion_vacia(juego->mapa[1], i, j) ==
						false,
					"la posicion esta siendo ocupada en (%i,%i) en el mapa del jugador 2",
					i, j);
			}
		}
	}

	destruir_juego(juego);
	pokedex_destruir(p);
}

// pruebas de menu

void pruebas_de_creacion_de_menu()
{
	menu_t *m = crear_menu("Menu prueba");
	pa2m_afirmar(m != NULL, "se creo un menu correctamente");

	destruir_menu(m);
	pa2m_afirmar(true, "se destruyo correctamente el menu");

	m = crear_menu(NULL);
	pa2m_afirmar(m == NULL,
		     " no es posible crear un menu con un nombre null");
}

bool saludar(void *ctx)
{
	printf("Hola mundo\n");
	return true;
}

bool despedirse(void *ctx)
{
	printf("Adios mundo\n");
	return true;
}

bool funcion_generica(void *ctx)
{
	printf("Funcion generica\n");
	return true;
}

void pruebas_de_creacion_de_operaciones()
{
	operacion_t *op_1 = crear_operacion("Saludar", saludar);
	operacion_t *op_2 = crear_operacion("Despedirser", despedirse);
	operacion_t *op_3 = crear_operacion(NULL, despedirse);
	operacion_t *op_4 = crear_operacion("Operacion sin nombre", NULL);

	pa2m_afirmar(op_1 != NULL, "se creo una operacion correctamente");
	pa2m_afirmar(op_2 != NULL, "se creo una operacion correctamente");
	pa2m_afirmar(op_3 == NULL,
		     "no se puede crear una operacion que no tenga nombre");
	pa2m_afirmar(op_4 == NULL,
		     "no se puede crear una operacion que no funcion");

	destruir_operacion(op_1);
	destruir_operacion(op_2);
	pa2m_afirmar(true, "se destruyo correctamente las operaciones creadas");
}

void pruebas_de_agregar_operaciones()
{
	menu_t *m = crear_menu("Menu prueba");
	operacion_t *op_1 = crear_operacion("Saludar", saludar);
	operacion_t *op_2 = crear_operacion("Despedirser", despedirse);
	operacion_t *op_3 = crear_operacion(NULL, despedirse);
	operacion_t *op_4 = crear_operacion("Operacion sin nombre", NULL);
	pa2m_afirmar(menu_agregar_operacion(m, op_1, "S"),
		     "se agrego una operacon al menu");
	pa2m_afirmar(menu_agregar_operacion(m, op_2, "D"),
		     "se agrego una operacon al menu");
	pa2m_afirmar(cantidad_operaciones(m) == 2,
		     "actualmente hay 2 operaciones en el menu");
	pa2m_afirmar(menu_agregar_operacion(m, op_3, "n") == false,
		     "No se puede agregar una operacion null al menu");
	pa2m_afirmar(menu_agregar_operacion(m, op_4, "n") == false,
		     "No se puede agregar una operacion null al menu");

	destruir_menu(m);
}

void pruebas_de_eliminacion_de_operaciones()
{
	menu_t *m = crear_menu("Menu prueba");
	operacion_t *op_1 = crear_operacion("Saludar", saludar);
	operacion_t *op_2 = crear_operacion("Despedirser", despedirse);
	operacion_t *op_3 = crear_operacion("Generico", funcion_generica);

	pa2m_afirmar(menu_agregar_operacion(m, op_1, "S"),
		     "se agrego una operacon al menu");
	pa2m_afirmar(menu_agregar_operacion(m, op_2, "D"),
		     "se agrego una operacon al menu");
	pa2m_afirmar(menu_agregar_operacion(m, op_3, "G"),
		     "se agrego una operacon al menu");
	pa2m_afirmar(cantidad_operaciones(m) == 3,
		     "hay tres operaciones actualmente en el menu");
	pa2m_afirmar(menu_eliminar_operacion(m, "S") != NULL,
		     "se elimino la operacion");
	pa2m_afirmar(cantidad_operaciones(m) == 2,
		     "se redujo la cantidad de operaciones en el menu");
	pa2m_afirmar(menu_eliminar_operacion(m, "D") != NULL,
		     "se elimino la operacion");
	pa2m_afirmar(cantidad_operaciones(m) == 1,
		     "se redujo la cantidad de operaciones en el menu");
	pa2m_afirmar(menu_eliminar_operacion(m, "G") != NULL,
		     "se elimino la operacion");
	pa2m_afirmar(cantidad_operaciones(m) == 0,
		     "se redujo la cantidad de operaciones en el menu");

	destruir_operacion(op_1);
	destruir_operacion(op_2);
	destruir_operacion(op_3);

	destruir_menu(m);
}

void pruebas_de_ejecutar_operaciones()
{
	menu_t *m = crear_menu("Menu prueba");
	operacion_t *op_1 = crear_operacion("Saludar", saludar);
	operacion_t *op_2 = crear_operacion("Despedirser", despedirse);
	operacion_t *op_3 = crear_operacion("Generico", funcion_generica);

	pa2m_afirmar(menu_agregar_operacion(m, op_1, "S"),
		     "se agrego una operacion al menu");
	pa2m_afirmar(menu_agregar_operacion(m, op_2, "D"),
		     "se agrego una operacion al menu");
	pa2m_afirmar(menu_agregar_operacion(m, op_3, "G"),
		     "se agrego una operacion al menu");

	pa2m_afirmar(ejecutar_operacion(m, "S", NULL),
		     "Se ejecuto correctamente la operacion");
	pa2m_afirmar(ejecutar_operacion(m, "D", NULL),
		     "Se ejecuto correctamente la operacion");
	pa2m_afirmar(ejecutar_operacion(m, "G", NULL),
		     "Se ejecuto correctamente la operacion");

	pa2m_afirmar(ejecutar_operacion(m, "j", NULL) == false,
		     "No se puede ejecutar una operacion inexistente");
	pa2m_afirmar(ejecutar_operacion(NULL, "j", NULL) == false,
		     "No se puede ejecutar una operacion en un menu NULL");

	destruir_menu(m);
}
int main()
{
	pa2m_nuevo_grupo(
		"============== prueba de creacion de mapa ===============");
	creacion_y_destrucion_de_mapa();
	pa2m_nuevo_grupo(
		"============== prueba de insercion de objetos al mapa ===============");
	pruebas_de_insertar_objetos_en_mapa();
	pa2m_nuevo_grupo(
		"============== prueba extracion y obtecion de objetos ===============");
	pruebas_de_obtener_y_extracion_objetos_en_mapa();
	pa2m_nuevo_grupo(
		"============== pruebas de creacion y destruccion de jugador");
	pruebas_de_creacion_y_destrucion_de_jugador();
	pa2m_nuevo_grupo(
		"============== pruebas de movimiento de jugador ===============");
	pruebas_de_movimiento_de_jugador();
	pa2m_nuevo_grupo(
		"============== pruebas de operaciones de jugador ===============");
	prueba_operaciones_de_jugador();
	pa2m_nuevo_grupo("============== pruebas de pokemones ===============");
	pruebas_de_crecion_y_destrucion_de_pokemones();
	pa2m_nuevo_grupo(
		"============== pruebas de creacion y destrucion de los datos del juego ===============");
	pruebas_de_creacion_y_destrucion_de_los_datos_del_juego();
	pa2m_nuevo_grupo(
		"============== pruebas de cargar datos al juego ===============");
	prueba_de_cargar_pokemones_al_juego();
	pa2m_nuevo_grupo(
		"=========== pruebas de creacion y destrucion de menu========");
	pruebas_de_creacion_de_menu();
	pa2m_nuevo_grupo(
		"=========== pruebas de creacion y destrucion de operaciones========");
	pruebas_de_creacion_de_operaciones();
	pa2m_nuevo_grupo(
		"=========== pruebas de agregacion de operaciones al menu ========");
	pruebas_de_agregar_operaciones();
	pa2m_nuevo_grupo(
		"=========== pruebas de eliminacion de operaciones al menu ========");
	pruebas_de_eliminacion_de_operaciones();
	pa2m_nuevo_grupo(
		"=========== pruebas de ejecutar operaciones al menu ========");
	pruebas_de_ejecutar_operaciones();
	return pa2m_mostrar_reporte();
}
