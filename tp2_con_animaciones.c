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

//ESTADOS EN LAS ANIMACIONES
typedef enum {
	MOSTRAR_INICIO,
	MOSTRAR_CARGA,
	MOSTRAR_MENU,
	MOSTRAR_SUB_MENU_MOSTRAR,
	MOSTRAR_SUB_MENU_BUSCAR,
	MOSTRAR_PAUSA,
	FINALIZAR
} estado_t;

typedef struct contexto_animacion {
	estado_t estado; // ESTADO EN EL QUE ESTA
	estado_t estado_menu; // ESTADO DE MENU
	size_t frame;
} contexto_animacion_t;

typedef struct contexto_menu {
	pokedex_t *p;
	contexto_animacion_t *animacion_ctx;
	int tecla;
	menu_t *menu_principal;
	menu_t *sub_menu_mostrar;
	menu_t *sub_menu_buscar;
} contexto_menu_t;

// ===================== ANIMACIONES ==========================

void mostrar_frame_inicio(size_t frame)
{
	borrar_pantalla();
	switch (frame % 3) {
	case 0:
		inicio_frame_1();
		break;
	case 1:
		inicio_frame_2();
		break;
	case 2:
		inicio_frame_3();
		break;
	}
}

void mostrar_frame_carga(size_t frame)
{
	borrar_pantalla();
	switch (frame) {
	case 0:
		mostrar_carga_frame_1();
		break;
	case 1:
		mostrar_carga_frame_2();
		break;
	case 2:
		mostrar_carga_frame_3();
		break;
	}
}

int animacion_carga_inicio_callback(int tecla, void *datos)
{
	contexto_menu_t *menu_ctx = datos;
	contexto_animacion_t *ctx = menu_ctx->animacion_ctx;

	int estado_actual = ctx->estado;
	switch (estado_actual) {
	case MOSTRAR_INICIO:
		if (tecla == ENTER) {
			ctx->estado = MOSTRAR_CARGA;
			ctx->estado_menu = MOSTRAR_MENU;
			ctx->frame = 0;
		} else if (tecla == 'Q' || tecla == 'q') {
			ctx->estado = MOSTRAR_CARGA;
			ctx->estado_menu = FINALIZAR;
			menu_ctx->tecla = tecla;
			ctx->frame = 0;
			return -1;
		} else {
			mostrar_frame_inicio(ctx->frame);
			ctx->frame++;
		}
		break;
	case MOSTRAR_CARGA:
		if (ctx->frame < 15) {
			mostrar_frame_carga((ctx->frame / 5) % 3);
			ctx->frame++;
		} else {
			ctx->estado = FINALIZAR;
			ctx->estado_menu = ctx->estado_menu;
			ctx->frame = 0;
		}
		break;
	case FINALIZAR:
		borrar_pantalla();
		return 1;
	default:
		return 0;
		break;
	}
	return 0;
}

int animacion_menu_callback(int tecla, void *datos)
{
	contexto_menu_t *ctx_pokedex = datos;
	contexto_animacion_t *ctx = ctx_pokedex->animacion_ctx;

	int estado_actual = ctx->estado;
	switch (estado_actual) {
	case MOSTRAR_CARGA:

		if (ctx->frame < 15) {
			mostrar_frame_carga((ctx->frame / 5) % 3);
			ctx->frame++;
		} else {
			ctx->frame = 0;
			ctx->estado = ctx->estado_menu;
		}
		break;

	case MOSTRAR_MENU:
		borrar_pantalla();
		mostrar_menu();
		if (tecla == 'J' || tecla == 'j') {
			ctx->estado = FINALIZAR;
			ctx_pokedex->tecla = tecla;
		} else if (tecla == 'S' || tecla == 's') {
			ctx->estado = FINALIZAR;
			ctx_pokedex->tecla = tecla;
		} else if (tecla == 'M' || tecla == 'm') {
			ctx->estado = MOSTRAR_CARGA;
			ctx->estado_menu = MOSTRAR_SUB_MENU_MOSTRAR;

		} else if (tecla == 'B' || tecla == 'b') {
			ctx->estado = MOSTRAR_CARGA;
			ctx->estado_menu = MOSTRAR_SUB_MENU_BUSCAR;

		} else if (tecla == 'Q' || tecla == 'q') {
			ctx->estado = MOSTRAR_CARGA;
			ctx->estado_menu = FINALIZAR;
			ctx->frame = 0;
		} else {
			ctx->estado = MOSTRAR_MENU;
		}
		break;
	case MOSTRAR_SUB_MENU_BUSCAR:
		borrar_pantalla();
		mostrar_sub_menu_buscar();

		if (tecla == 'N' || tecla == 'n') {
			ctx->estado = FINALIZAR;
			ctx_pokedex->tecla = tecla;

		} else if (tecla == 'I' || tecla == 'i') {
			ctx->estado = FINALIZAR;
			ctx_pokedex->tecla = tecla;
		} else if (tecla == 'A' || tecla == 'a') {
			ctx->estado = MOSTRAR_CARGA;
			ctx->estado_menu = MOSTRAR_MENU;
		}
		break;
	case MOSTRAR_SUB_MENU_MOSTRAR:
		borrar_pantalla();
		mostrar_sub_menu_mostrar();

		if (tecla == 'N' || tecla == 'n') {
			ctx->estado = FINALIZAR;
			ctx_pokedex->tecla = tecla;
		} else if (tecla == 'I' || tecla == 'i') {
			ctx->estado = FINALIZAR;
			ctx_pokedex->tecla = tecla;
		} else if (tecla == 'A' || tecla == 'a') {
			ctx->estado = MOSTRAR_CARGA;
			ctx->estado_menu = MOSTRAR_MENU;
		}

		break;
	case MOSTRAR_PAUSA:

		if (tecla == ENTER) {
			ctx->estado = ctx->estado_menu;
		}
		break;
	case FINALIZAR:

		return 1;
	}

	return 0;
}

// ===================== MENU POKEDEX =========================
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
		c->animacion_ctx->estado = MOSTRAR_PAUSA;
		return true;
	}

	const struct pokemon *p =
		pokedex_buscar_pokemon_id(c->p, (unsigned int)id);
	if (p != NULL) {
		imprimir_pokemon(p);
	} else {
		imprimir_no_encontrado();
	}

	c->animacion_ctx->estado = MOSTRAR_PAUSA;
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
		c->animacion_ctx->estado = MOSTRAR_PAUSA;
		return true;
	}
	const struct pokemon *p = pokedex_buscar_pokemon_nombre(c->p, nombre);
	if (p != NULL) {
		imprimir_pokemon(p);
	} else {
		imprimir_no_encontrado();
	}
	c->animacion_ctx->estado = MOSTRAR_PAUSA;
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
	printf(ANSI_COLOR_BOLD "\t\t _______________________________\n");
	printf(ANSI_COLOR_BOLD
	       "\t\t|presiona " ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
	       "enter " ANSI_COLOR_RESET ANSI_COLOR_BOLD " para continuar|\n");
	printf(ANSI_COLOR_BOLD "\t\t ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");
	c->animacion_ctx->estado = MOSTRAR_PAUSA;
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
	printf(ANSI_COLOR_BOLD "\t\t _______________________________\n");
	printf(ANSI_COLOR_BOLD
	       "\t\t|presiona " ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
	       "enter " ANSI_COLOR_RESET ANSI_COLOR_BOLD " para continuar|\n");
	printf(ANSI_COLOR_BOLD "\t\t ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");

	c->animacion_ctx->estado = MOSTRAR_PAUSA;
	return true;
}

bool salir_de_la_pokedex(void *ctx)
{
	contexto_menu_t *c = ctx;
	contexto_animacion_t *animacion_ctx = c->animacion_ctx;
	c->animacion_ctx->estado = MOSTRAR_PAUSA;

	animacion_ctx->estado_menu = MOSTRAR_MENU;

	return false;
}

bool iniciar_juego(void *ctx)
{
	contexto_menu_t *c = ctx;
	int semilla = (int)time(NULL);
	main_juego(c->p, &semilla);
	borrar_pantalla();
	contexto_animacion_t *animacion_ctx = c->animacion_ctx;

	animacion_ctx->estado = MOSTRAR_PAUSA;
	animacion_ctx->frame = 0;

	return true;
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
		c->animacion_ctx->estado = MOSTRAR_PAUSA;
		return true;
	}
	main_juego(c->p, &semilla);
	borrar_pantalla();

	contexto_animacion_t *animacion_ctx = c->animacion_ctx;
	animacion_ctx->estado = MOSTRAR_PAUSA;
	animacion_ctx->frame = 0;

	return true;
}

bool iniciar_sub_menu_mostrar(void *ctx)
{
	contexto_menu_t *c = ctx;
	contexto_animacion_t *animacion_ctx = c->animacion_ctx;
	animacion_ctx->estado = MOSTRAR_CARGA;
	animacion_ctx->estado_menu = MOSTRAR_SUB_MENU_MOSTRAR;
	return true;
}

bool iniciar_sub_menu_buscar(void *ctx)
{
	contexto_menu_t *c = ctx;
	contexto_animacion_t *animacion_ctx = c->animacion_ctx;
	animacion_ctx->estado = MOSTRAR_CARGA;
	animacion_ctx->estado_menu = MOSTRAR_SUB_MENU_BUSCAR;
	return true;
}

// ===INICIAR MENU===
void menu_iniciar(void *ctx)
{
	contexto_menu_t *ctx_pokedex = ctx;
	contexto_animacion_t *ctx_animacion = ctx_pokedex->animacion_ctx;
	game_loop(animacion_carga_inicio_callback, ctx_pokedex);

	if (ctx_pokedex->tecla == 'Q' || ctx_pokedex->tecla == 'q') {
		return;
	}

	ctx_animacion->estado = ctx_animacion->estado_menu;
	int estado_actual = ctx_animacion->estado;
	char tecla[2];
	while (estado_actual != FINALIZAR) {
		game_loop(animacion_menu_callback, ctx_pokedex);

		tecla[0] = (char)toupper(ctx_pokedex->tecla);
		tecla[1] = '\0';
		if (ctx_animacion->estado_menu == MOSTRAR_MENU) {
			ejecutar_operacion(ctx_pokedex->menu_principal, tecla,
					   ctx_pokedex);
		} else if (ctx_animacion->estado_menu ==
			   MOSTRAR_SUB_MENU_MOSTRAR) {
			ejecutar_operacion(ctx_pokedex->sub_menu_mostrar, tecla,
					   ctx_pokedex);
		} else if (ctx_animacion->estado_menu ==
			   MOSTRAR_SUB_MENU_BUSCAR) {
			ejecutar_operacion(ctx_pokedex->sub_menu_buscar, tecla,
					   ctx_pokedex);
		}

		estado_actual = ctx_animacion->estado;
	}
}

// ===================== MAIN =========================

int main(int argc, char *argv[])
{
	if (argc <= 1) {
		printf("Falta que pases un archivo");
		return -1;
	}
	esconder_cursor();

	contexto_animacion_t anim_ctx = { .estado = MOSTRAR_INICIO,
					  .estado_menu = MOSTRAR_MENU,
					  .frame = 0 };

	pokedex_t *p = pokedex_abrir(argv[1]);
	menu_t *menu_pokedex = crear_menu("Menu pokedex");

	contexto_menu_t contexto_pokedex = {
		.p = p,
		.menu_principal = menu_pokedex,
		.sub_menu_mostrar = crear_menu("Menu Mostrar"),
		.sub_menu_buscar = crear_menu("Menu Buscar"),
		.animacion_ctx = &anim_ctx,
		.tecla = 'l'
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

	menu_iniciar(&contexto_pokedex);
	borrar_pantalla();

	printf(ANSI_COLOR_BOLD
	       "\t Termino el programa gracias por usarlo =) \n" ANSI_COLOR_RESET);

	destruir_menu(menu_pokedex);
	destruir_menu(contexto_pokedex.sub_menu_buscar);
	destruir_menu(contexto_pokedex.sub_menu_mostrar);
	pokedex_destruir(p);
	mostrar_cursor();
	return 0;
}