#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "extra/engine.h"
#include "extra/ansi.h"
#include "src/pokedex.h"
#include "src/menu.h"
#include "src/assets.h"
#include "src/juego_logica.h"
#include "src/pokemones.h"
#include <ctype.h>

#define ENTER 10

typedef struct contexto_menu {
	pokedex_t *p;
	menu_t *menu_principal;
	menu_t *sub_menu_mostrar;
	menu_t *sub_menu_buscar;
} contexto_menu_t;

bool buscar_pokemon_por_id(void *ctx)
{
	contexto_menu_t *c = ctx;
	int id;
	printf(ANSI_COLOR_BOLD "\tbusca un pokemon por id: " ANSI_COLOR_RESET);

	int leidos = scanf("%i", &id);
	getchar();

	if (leidos != 1) {
		fprintf(stderr, "Error: entrada inválida.\n");
		while (getchar() != '\n')
			;
		return true;
	}

	const struct pokemon *p =
		pokedex_buscar_pokemon_id(c->p, (unsigned int)id);
	if (p != NULL) {
		imprimir_pokemon(p);
	} else {
		imprimir_no_encontrado();
	}
	return true;
}

bool buscar_pokemon_por_nombre(void *ctx)
{
	contexto_menu_t *c = ctx;
	char nombre[100];

	printf(ANSI_COLOR_BOLD
	       "\tbusca un pokemon por nombre: " ANSI_COLOR_RESET);

	int leidos = scanf("%s", nombre);
	getchar();

	if (leidos != 1) {
		fprintf(stderr, "Error: entrada inválida.\n");
		while (getchar() != '\n')
			;
		return true;
	}
	const struct pokemon *p = pokedex_buscar_pokemon_nombre(c->p, nombre);
	if (p != NULL) {
		imprimir_pokemon(p);
	} else {
		imprimir_no_encontrado();
	}

	return true;
}

void mostrar_pokemon_nombre_y_tipo_(struct pokemon *poke)
{
	tipo_pokemon tipo = poke->tipo;
	switch (tipo) {
	case TIPO_AGUA:
		printf(ANSI_COLOR_BOLD
		       "| NOMBRE POKEMON: " ANSI_COLOR_BLUE ANSI_COLOR_BOLD
		       " %s "
		       "ID :%i " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_BLUE ANSI_COLOR_BOLD
		       "AGUA🫧\t" ANSI_COLOR_RESET ANSI_COLOR_BOLD "FUERZA: %i "
		       "INTELIGENCIA: %i "
		       "DESTREZA: %i \t|\n" ANSI_COLOR_RESET,
		       poke->nombre, poke->id, poke->fuerza, poke->inteligencia,
		       poke->destreza);
		break;
	case TIPO_ELECTRICO:
		printf(ANSI_COLOR_BOLD
		       "| NOMBRE POKEMON: " ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
		       " %s "
		       "ID :%i " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
		       "ELECTRICO ⚡️\t" ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "FUERZA: %i "
		       "INTELIGENCIA: %i "
		       "DESTREZA: %i \t|\n" ANSI_COLOR_RESET,
		       poke->nombre, poke->id, poke->fuerza, poke->inteligencia,
		       poke->destreza);
		break;
	case TIPO_FUEGO:
		printf(ANSI_COLOR_BOLD
		       "| NOMBRE POKEMON: " ANSI_COLOR_RED ANSI_COLOR_BOLD
		       " %s "
		       "ID :%i " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_RED ANSI_COLOR_BOLD
		       "FUEGO 🔥\t" ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "FUERZA: %i "
		       "INTELIGENCIA: %i "
		       "DESTREZA: %i \t|\n" ANSI_COLOR_RESET,
		       poke->nombre, poke->id, poke->fuerza, poke->inteligencia,
		       poke->destreza);
		break;
	case TIPO_NORMAL:
		printf(ANSI_COLOR_BOLD
		       "| NOMBRE POKEMON: " ANSI_COLOR_BOLD " %s "
		       "ID :%i " ANSI_COLOR_RESET "TIPO: " ANSI_COLOR_BOLD
		       "NORMAL 🌈\t" ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "FUERZA: %i "
		       "INTELIGENCIA: %i "
		       "DESTREZA: %i \t|\n" ANSI_COLOR_RESET,
		       poke->nombre, poke->id, poke->fuerza, poke->inteligencia,
		       poke->destreza);
		break;
	case TIPO_PLANTA:
		printf(ANSI_COLOR_BOLD
		       "| NOMBRE POKEMON: " ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       " %s "
		       "ID :%i " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       "PLATA 🍀 \t" ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "FUERZA: %i "
		       "INTELIGENCIA: %i "
		       "DESTREZA: %i \t|\n" ANSI_COLOR_RESET,
		       poke->nombre, poke->id, poke->fuerza, poke->inteligencia,
		       poke->destreza);
		break;
	case TIPO_ROCA:
		printf(ANSI_COLOR_BOLD
		       "| NOMBRE POKEMON: " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       " %s "
		       "ID :%i " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "ROCA 🪨\t" ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "FUERZA: %i "
		       "INTELIGENCIA: %i "
		       "DESTREZA: %i \t|\n" ANSI_COLOR_RESET,
		       poke->nombre, poke->id, poke->fuerza, poke->inteligencia,
		       poke->destreza);
		break;
	case TIPO_LUCHA:
		printf(ANSI_COLOR_BOLD
		       "| NOMBRE POKEMON: " ANSI_COLOR_CYAN ANSI_COLOR_BOLD
		       " %s "
		       "ID :%i " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_CYAN ANSI_COLOR_BOLD
		       "LUCHA 🥊\t" ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "FUERZA: %i "
		       "INTELIGENCIA: %i "
		       "DESTREZA: %i \t|\n" ANSI_COLOR_RESET,
		       poke->nombre, poke->id, poke->fuerza, poke->inteligencia,
		       poke->destreza);
		break;
	default:
		break;
	}
}

bool mostrar_pokemon(struct pokemon *p, void *ctx)
{
	if (p == NULL) {
		return false;
	}
	mostrar_pokemon_nombre_y_tipo_(p);
	return true;
}

bool mostrar_pokemones_por_nombre(void *ctx)
{
	contexto_menu_t *c = ctx;
	printf(ANSI_COLOR_BOLD
	       "+-------------------------------------------------------------------------------------------------------+\n" ANSI_COLOR_RESET);
	pokedex_iterar_pokemones(c->p, ITERAR_NOMBRE, mostrar_pokemon, NULL);
	printf(ANSI_COLOR_BOLD
	       "+-------------------------------------------------------------------------------------------------------+\n\n" ANSI_COLOR_RESET);

	return true;
}

bool mostrar_pokemones_por_id(void *ctx)
{
	contexto_menu_t *c = ctx;
	printf(ANSI_COLOR_BOLD
	       "+-------------------------------------------------------------------------------------------------------+\n" ANSI_COLOR_RESET);
	pokedex_iterar_pokemones(c->p, ITERAR_ID, mostrar_pokemon, NULL);
	printf(ANSI_COLOR_BOLD
	       "+-------------------------------------------------------------------------------------------------------+\n" ANSI_COLOR_RESET);

	return true;
}

bool salir_de_la_pokedex(void *ctx)
{
	//contexto_menu_t *c = ctx;
	borrar_pantalla();
	return false;
}

bool iniciar_juego(void *ctx)
{
	contexto_menu_t *c = ctx;
	int semilla = (int)time(NULL);
	main_juego(c->p, &semilla);

	int palabra;
	int leido = scanf("%i", &palabra);
	if (leido >= 1) {
		borrar_pantalla();
	}
	borrar_pantalla();

	return false;
}

bool iniciar_juego_con_semilla(void *ctx)
{
	contexto_menu_t *c = ctx;
	printf(ANSI_COLOR_BOLD
	       " Elegi una semilla para empezar el juego: " ANSI_COLOR_RESET);

	int semilla;
	int leidos = scanf("%i", &semilla);
	getchar();

	if (leidos != 1) {
		fprintf(stderr, "Error: entrada inválida.\n");
		while (getchar() != '\n')
			;
		return true;
	}
	main_juego(c->p, &semilla);

	int palabra;
	int leido = scanf("%i", &palabra);
	if (leido >= 1) {
		borrar_pantalla();
	}

	return false;
}

bool iniciar_sub_menu_mostrar(void *ctx)
{
	//contexto_menu_t *c = ctx;

	contexto_menu_t *c = ctx;

	borrar_pantalla();
	iniciar_menu(c->sub_menu_mostrar, c);

	return true;
}

bool iniciar_sub_menu_buscar(void *ctx)
{
	contexto_menu_t *c = ctx;

	borrar_pantalla();
	iniciar_menu(c->sub_menu_buscar, c);

	return true;
}

int main(int argc, char *argv[])
{
	if (argc <= 1) {
		printf("Falta que pases un archivo");
		return -1;
	}
	esconder_cursor();

	pokedex_t *p = pokedex_abrir(argv[1]);
	menu_t *menu_pokedex = crear_menu("Menu pokedex");

	contexto_menu_t contexto_pokedex = {
		.p = p,
		.menu_principal = menu_pokedex,
		.sub_menu_mostrar = crear_menu("Menu Mostrar"),
		.sub_menu_buscar = crear_menu("Menu Buscar"),
	};

	// Submenu Mostrar
	menu_agregar_operacion(
		contexto_pokedex.sub_menu_mostrar,
		crear_operacion("Menu Anterior", salir_de_la_pokedex), "A");
	menu_agregar_operacion(contexto_pokedex.sub_menu_mostrar,
			       crear_operacion("Mostrar pokemones por nombre",
					       mostrar_pokemones_por_nombre),
			       "N");
	menu_agregar_operacion(contexto_pokedex.sub_menu_mostrar,
			       crear_operacion("Mostrar pokemones por id",
					       mostrar_pokemones_por_id),
			       "I");

	// Submenu Buscar
	menu_agregar_operacion(
		contexto_pokedex.sub_menu_buscar,
		crear_operacion("Menu Anterior", salir_de_la_pokedex), "A");
	menu_agregar_operacion(contexto_pokedex.sub_menu_buscar,
			       crear_operacion("Buscar pokemon por id",
					       buscar_pokemon_por_id),
			       "I");
	menu_agregar_operacion(contexto_pokedex.sub_menu_buscar,
			       crear_operacion("Buscar pokemon por nombre",
					       buscar_pokemon_por_nombre),
			       "N");

	// Menu principal
	menu_agregar_operacion(menu_pokedex,
			       crear_operacion("Jugar", iniciar_juego), "J");
	menu_agregar_operacion(menu_pokedex,
			       crear_operacion("Jugar con semilla",
					       iniciar_juego_con_semilla),
			       "S");
	menu_agregar_operacion(
		menu_pokedex,
		crear_operacion("Mostrar", iniciar_sub_menu_mostrar), "M");
	menu_agregar_operacion(
		menu_pokedex,
		crear_operacion("Buscar", iniciar_sub_menu_buscar), "B");
	menu_agregar_operacion(menu_pokedex,
			       crear_operacion("Salir de la pokedex",
					       salir_de_la_pokedex),
			       "Q");

	iniciar_menu(menu_pokedex, &contexto_pokedex);
	printf(ANSI_COLOR_BOLD
	       "\t Termino el programa gracias por usarlo =) \n" ANSI_COLOR_RESET);

	destruir_menu(menu_pokedex);
	destruir_menu(contexto_pokedex.sub_menu_buscar);
	destruir_menu(contexto_pokedex.sub_menu_mostrar);
	pokedex_destruir(p);
	mostrar_cursor();
	return 0;
}