#include "juego_logica.h"
#include "ansi.h"
#include "../extra/engine.h"
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#define GRAFICOS_V1 1
#define GRAFICOS_V2 2

/* 
logica contexto
*/
typedef struct {
	juego_t *j;
	int jugador1_x, jugador1_y;
	int jugador2_x, jugador2_y;
	int indice_rival;
	bool juego_iniciado;
	int graficos_mapa;
} logica_ctx_t;

void mostrar_pokemon_nombre_y_tipo(pokemon_juego_t *poke)
{
	tipo_pokemon tipo = poke->p->tipo;
	switch (tipo) {
	case TIPO_AGUA:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_BLUE ANSI_COLOR_BOLD
		       " %s " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_BLUE ANSI_COLOR_BOLD
		       "AGUA🫧\n" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	case TIPO_ELECTRICO:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
		       " %s " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
		       "ELECTRICO ⚡️\n" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	case TIPO_FUEGO:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_RED ANSI_COLOR_BOLD
		       " %s " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_RED ANSI_COLOR_BOLD
		       "FUEGO 🔥\n" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	case TIPO_NORMAL:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_BOLD " %s " ANSI_COLOR_RESET
		       "TIPO: " ANSI_COLOR_BOLD "NORMAL 🌈⃤\n" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	case TIPO_PLANTA:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       " %s " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       "PLATA 🍀\n" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	case TIPO_ROCA:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       " %s " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "ROCA 🪨\n" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	case TIPO_LUCHA:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_CYAN ANSI_COLOR_BOLD
		       " %s " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_CYAN ANSI_COLOR_BOLD
		       "LUCHA 🥊\n" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	default:
		break;
	}
}

bool lista_mostrar_pokemon(void *elemento, void *ctx)
{
	if (elemento == NULL) {
		return false;
	}
	pokemon_juego_t *poke = elemento;
	mostrar_pokemon_nombre_y_tipo(poke);

	return true;
}

void mostrar_stats_jugador(jugador_t *j)
{
	printf(ANSI_COLOR_BOLD
	       "+-----------------------------------------------+\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_BOLD
	       "|\t\tPOKEMONES CAPTURADOS\t\t|\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_BOLD
	       "+-----------------------------------------------+\n" ANSI_COLOR_RESET);

	lista_iterar(j->pokes_capturados, lista_mostrar_pokemon, NULL);
	printf(ANSI_COLOR_BOLD
	       "_________________________________________________\n" ANSI_COLOR_RESET);
	pokemon_juego_t *poke = pila_tope(j->acumulados);
	if (poke != NULL) {
		printf(ANSI_COLOR_BOLD
		       "\t\t+----------------+\n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_BOLD
		       "\t\t|PUNTOS TOTAL %i  |\n" ANSI_COLOR_RESET,
		       obtener_puntos_jugador(j));
		printf(ANSI_COLOR_BOLD
		       "\t\t+----------------+\n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_BOLD
		       "+--------------------------------------------------------------+\n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_BOLD "ULTIMO ACUMULADO" ANSI_COLOR_RESET);
		mostrar_pokemon_nombre_y_tipo(poke);
		printf(ANSI_COLOR_BOLD
		       "+--------------------------------------------------------------+\n" ANSI_COLOR_RESET);

	} else {
		printf(ANSI_COLOR_BOLD
		       "\t\t+----------------+\n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_BOLD
		       "\t\t|PUNTOS TOTAL: %i|\n" ANSI_COLOR_RESET,
		       obtener_puntos_jugador(j));
		printf(ANSI_COLOR_BOLD
		       "\t\t+----------------+\n" ANSI_COLOR_RESET);
	}
}

void imprimir_por_tipo(pokemon_juego_t *poke)
{
	tipo_pokemon tipo = poke->p->tipo;
	switch (tipo) {
	case TIPO_AGUA:
		printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD " %c " ANSI_COLOR_RESET,
		       poke->p->nombre[0]);
		break;
	case TIPO_ELECTRICO:
		printf(ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
		       " %c " ANSI_COLOR_RESET,
		       poke->p->nombre[0]);
		break;
	case TIPO_FUEGO:
		printf(ANSI_COLOR_RED ANSI_COLOR_BOLD " %c " ANSI_COLOR_RESET,
		       poke->p->nombre[0]);
		break;
	case TIPO_NORMAL:
		printf(ANSI_COLOR_BOLD " %c " ANSI_COLOR_RESET,
		       poke->p->nombre[0]);
		break;
	case TIPO_PLANTA:
		printf(ANSI_COLOR_GREEN ANSI_COLOR_BOLD " %c " ANSI_COLOR_RESET,
		       poke->p->nombre[0]);
		break;
	case TIPO_ROCA:
		printf(ANSI_COLOR_RESET ANSI_COLOR_BOLD " %c " ANSI_COLOR_RESET,
		       poke->p->nombre[0]);
		break;
	case TIPO_LUCHA:
		printf(ANSI_COLOR_CYAN " %c " ANSI_COLOR_RESET,
		       poke->p->nombre[0]);
		break;
	default:
		printf(" %c ", poke->p->nombre[0]);
		break;
	}
}
/*
mprimir dos mapas lado a lado 
*/
void imprimir_dos_mapas(mapa_t *m1, mapa_t *m2)
{
	// borde superior de ambos

	printf(ANSI_COLOR_BOLD
	       "TABLERO JUGADOR 1\t\t\t\t\t\tTABLERO JUGADOR 2\n" ANSI_COLOR_RESET);
	for (int t = 0; t < 2; t++) {
		for (int col = 0; col < MAX_COLUM + 2; col++)
			printf("🌳 ");
		if (t == 0)
			printf("\t\t\t");
	}
	printf("\n");
	// filas
	for (int i = 0; i < MAX_FILA; i++) {
		// primer mapa
		printf("🌳 ");
		for (int j = 0; j < MAX_COLUM; j++) {
			if (lista_tamanio(m1->mapa[i][j]) == 0)
				printf("   ");
			else {
				pokemon_juego_t *pj = obtener_mapa(m1, i, j);
				if (pj->es_jugador)
					printf(ANSI_COLOR_BOLD
					       "@  " ANSI_COLOR_RESET);
				else
					imprimir_por_tipo(pj);
			}
		}
		printf("🌳    ");
		// segundo mapa
		printf("\t\t\t🌳 ");
		for (int j = 0; j < MAX_COLUM; j++) {
			if (lista_tamanio(m2->mapa[i][j]) == 0)
				printf("   ");
			else {
				pokemon_juego_t *pj = obtener_mapa(m2, i, j);
				if (pj->es_jugador)
					printf(ANSI_COLOR_BOLD
					       "@  " ANSI_COLOR_RESET);
				else
					imprimir_por_tipo(pj);
			}
		}
		printf("🌳\n");
	}
	// borde inferior
	for (int t = 0; t < 2; t++) {
		for (int col = 0; col < MAX_COLUM + 2; col++)
			printf("🌳 ");
		if (t == 0)
			printf("\t\t\t");
	}
	printf("\n");
}

void imprimir_dos_mapas_v2(mapa_t *m1, mapa_t *m2)
{
	// borde superior de ambos

	printf(ANSI_COLOR_BOLD
	       "TABLERO JUGADOR 1\t\t\t\t\t\tTABLERO JUGADOR 2\n" ANSI_COLOR_RESET);
	for (int t = 0; t < 2; t++) {
		for (int col = 0; col < MAX_COLUM + 2; col++)
			printf("🌳 ");
		if (t == 0)
			printf("\t\t\t");
	}
	printf("\n");
	// filas
	for (int i = 0; i < MAX_FILA; i++) {
		// primer mapa
		printf("🌳 ");
		for (int j = 0; j < MAX_COLUM; j++) {
			if (lista_tamanio(m1->mapa[i][j]) == 0)
				printf("🌿 ");
			else {
				pokemon_juego_t *pj = obtener_mapa(m1, i, j);
				if (pj->es_jugador)
					printf("👹 ");
				else
					imprimir_por_tipo(pj);
			}
		}
		printf("🌳    ");
		// segundo mapa
		printf("\t\t\t🌳 ");
		for (int j = 0; j < MAX_COLUM; j++) {
			if (lista_tamanio(m2->mapa[i][j]) == 0)
				printf("🌿 ");
			else {
				pokemon_juego_t *pj = obtener_mapa(m2, i, j);
				if (pj->es_jugador)
					printf("👻 ");
				else
					imprimir_por_tipo(pj);
			}
		}
		printf("🌳\n");
	}
	// borde inferior
	for (int t = 0; t < 2; t++) {
		for (int col = 0; col < MAX_COLUM + 2; col++)
			printf("🌳 ");
		if (t == 0)
			printf("\t\t\t");
	}
	printf("\n");
}
int max(int a, int b)
{
	return (a > b) ? a : b;
}

int min(int a, int b)
{
	return (a < b) ? a : b;
}

bool verificar_movimientos(int tecla, logica_ctx_t *ctx)
{
	if (tecla == 'W')
		ctx->jugador1_y = max(0, ctx->jugador1_y - 1);
	else if (tecla == 'S')
		ctx->jugador1_y = min(MAX_FILA - 1, ctx->jugador1_y + 1);
	else if (tecla == 'A')
		ctx->jugador1_x = max(0, ctx->jugador1_x - 1);
	else if (tecla == 'D')
		ctx->jugador1_x = min(MAX_COLUM - 1, ctx->jugador1_x + 1);
	else if (tecla == TECLA_ARRIBA)
		ctx->jugador2_y = max(0, ctx->jugador2_y - 1);
	else if (tecla == TECLA_ABAJO)
		ctx->jugador2_y = min(MAX_FILA - 1, ctx->jugador2_y + 1);
	else if (tecla == TECLA_IZQUIERDA)
		ctx->jugador2_x = max(0, ctx->jugador2_x - 1);
	else if (tecla == TECLA_DERECHA)
		ctx->jugador2_x = min(MAX_COLUM - 1, ctx->jugador2_x + 1);
	return true;
}

void mostrar_pokemon_nombre_y_tipo_actual(pokemon_juego_t *poke)
{
	if (poke == NULL) {
		printf(" ");
	}
	tipo_pokemon tipo = poke->p->tipo;
	switch (tipo) {
	case TIPO_AGUA:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_BLUE ANSI_COLOR_BOLD
		       " %s " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_BLUE ANSI_COLOR_BOLD
		       "AGUA🫧" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	case TIPO_ELECTRICO:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
		       " %s " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
		       "ELECTRICO ⚡️" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	case TIPO_FUEGO:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_RED ANSI_COLOR_BOLD
		       " %s " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_RED ANSI_COLOR_BOLD
		       "FUEGO 🔥" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	case TIPO_NORMAL:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_BOLD " %s " ANSI_COLOR_RESET
		       "TIPO: " ANSI_COLOR_BOLD "NORMAL 🌈⃤" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	case TIPO_PLANTA:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       " %s " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       "PLATA 🍀" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	case TIPO_ROCA:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       " %s " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "ROCA 🪨" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	case TIPO_LUCHA:
		printf(ANSI_COLOR_BOLD
		       "\tPOKEMON: " ANSI_COLOR_CYAN ANSI_COLOR_BOLD
		       " %s " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "TIPO: " ANSI_COLOR_CYAN ANSI_COLOR_BOLD
		       "LUCHA 🥊" ANSI_COLOR_RESET,
		       poke->p->nombre);
		break;
	default:
		break;
	}
}

void imprimir_acumulado(jugador_t *j)
{
	printf(ANSI_COLOR_YELLOW "  [Acumulado Tope]  " ANSI_COLOR_RESET);
	if (tiene_acumulado(j)) {
		pokemon_juego_t *poke = obtener_acumulado(j);
		mostrar_pokemon_nombre_y_tipo(poke);
	} else {
		printf(ANSI_COLOR_RED "VACÍO" ANSI_COLOR_RESET);
	}
	printf("\n");
}

void imprimir_datos_del_jugador(jugador_t *j, const char *nombre_jugador,
				const char *color)
{
	printf("\n" ANSI_COLOR_BOLD "%s"
	       " %s" ANSI_COLOR_BOLD " (%d,%d) | Puntos: %d" ANSI_COLOR_RESET
	       "\n",
	       color, nombre_jugador, j->x, j->y, obtener_puntos_jugador(j));

	pokemon_juego_t *ultimo_capturado = obtener_ultimo_capturado(j);
	if (ultimo_capturado != NULL) {
		printf(ANSI_COLOR_CYAN
		       "  [Último Capturado]  " ANSI_COLOR_RESET);
		mostrar_pokemon_nombre_y_tipo_actual(ultimo_capturado);
	} else {
		printf(ANSI_COLOR_RED
		       "  [Último Capturado] Ninguno" ANSI_COLOR_RESET);
	}

	imprimir_acumulado(j);
	printf(ANSI_COLOR_BOLD
	       "\n===============================================\n" ANSI_COLOR_RESET);
}

void imprimir_datos_actuales_de_los_jugadores(jugador_t *j_1, jugador_t *j_2)
{
	printf("\n=================== ¡INICIO DEL JUEGO! ===================\n");
	imprimir_datos_del_jugador(j_1, "Jugador 1", ANSI_COLOR_GREEN);
	imprimir_datos_del_jugador(j_2, "Jugador 2", ANSI_COLOR_CYAN);
	printf(ANSI_COLOR_BOLD
	       "==========================================================\n" ANSI_COLOR_RESET);
}

void procesar_movimiento(mapa_t *mapa, jugador_t *jugador, jugador_t *rival,
			 int nueva_y, int nueva_x)
{
	if (!mover_jugador(mapa, jugador, nueva_y, nueva_x)) {
		while (!posicion_vacia(mapa, nueva_y, nueva_x)) {
			pokemon_juego_t *poke_capturado = capturar_pokemon(
				mapa, jugador, nueva_y, nueva_x);
			agregar_acumulado(rival, poke_capturado);
			restar_puntos(rival);

			if (tiene_acumulado(jugador)) {
				pokemon_juego_t *tope =
					obtener_acumulado(jugador);
				if (verificar_tipo_poke(tope, poke_capturado)) {
					sacar_acumulado(jugador);
					sumar_puntos(jugador);
				}
			}
		}
		mover_jugador(mapa, jugador, nueva_y, nueva_x);
	}
}
// Callback para game_loop
int logica_callback(int tecla, void *data)
{
	logica_ctx_t *ctx = data;
	juego_t *j = ctx->j;

	tecla = toupper(tecla);
	if (ctx->juego_iniciado) {
		// salir con q o esc
		if (tecla == 27 || tecla == 'Q')
			ctx->juego_iniciado = false;
		if (verificar_movimientos(tecla, ctx)) {
			procesar_movimiento(j->mapa[0], j->jugadores[0],
					    j->jugadores[1], ctx->jugador1_y,
					    ctx->jugador1_x);
			procesar_movimiento(j->mapa[1], j->jugadores[1],
					    j->jugadores[0], ctx->jugador2_y,
					    ctx->jugador2_x);
		}

		// refersco la pantalla
		borrar_pantalla();

		imprimir_datos_actuales_de_los_jugadores(j->jugadores[0],
							 j->jugadores[1]);
		if (ctx->graficos_mapa == GRAFICOS_V1) {
			imprimir_dos_mapas(j->mapa[0], j->mapa[1]);
		} else if (ctx->graficos_mapa == GRAFICOS_V2) {
			imprimir_dos_mapas_v2(j->mapa[0], j->mapa[1]);
		}
		if (ctx->j->jugadores[0]->puntos <= 0 ||
		    ctx->j->jugadores[1]->puntos <= 0) {
			ctx->juego_iniciado = false;
		}
		if (j->tiempo_maximo_de_juego / 5 > 30) {
			printf(ANSI_COLOR_BOLD
			       "\t\t\t\t\t   TIEMPO RESTANTE: " ANSI_COLOR_GREEN ANSI_COLOR_BOLD
			       "%u"
			       "\n" ANSI_COLOR_RESET,
			       j->tiempo_maximo_de_juego / 5);
		} else if (j->tiempo_maximo_de_juego / 5 <= 30 &&
			   j->tiempo_maximo_de_juego / 5 > 10) {
			printf(ANSI_COLOR_BOLD
			       "\t\t\t\t\t   TIEMPO RESTANTE: " ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
			       "%u"
			       "\n" ANSI_COLOR_RESET,
			       j->tiempo_maximo_de_juego / 5);
		} else {
			printf(ANSI_COLOR_BOLD
			       "\t\t\t\t\t   TIEMPO RESTANTE: " ANSI_COLOR_RED ANSI_COLOR_BOLD
			       "%u"
			       "\n" ANSI_COLOR_RESET,
			       j->tiempo_maximo_de_juego / 5);
		}
		printf(ANSI_COLOR_BOLD
		       "\t\t\t\t\t   SEMILLA : " ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       "%u 🌱"
		       "\n" ANSI_COLOR_RESET,
		       j->semilla);
		j->tiempo_maximo_de_juego--;
		printf(ANSI_COLOR_BOLD
		       "\t\t\t\t   _______________________________\n");
		printf(ANSI_COLOR_BOLD
		       "\t\t\t\t  |presiona " ANSI_COLOR_RED ANSI_COLOR_BOLD
		       "Q" ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       " para salir del juego|\n");
		printf(ANSI_COLOR_BOLD
		       "\t\t\t\t   ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");

		if (j->tiempo_maximo_de_juego == 0) {
			ctx->juego_iniciado = false;
		}
	} else {
		return -1;
	}
	return 0; // continuar loop
}

void mostrar_resultados(jugador_t *j_1, jugador_t *j_2)
{
	if (obtener_puntos_jugador(j_1) > obtener_puntos_jugador(j_2)) {
		printf(ANSI_COLOR_BOLD "\t\t+----------------------+\n");
		printf(ANSI_COLOR_BOLD
		       "\t\t|" ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       "¡Ganó el Jugador 1 👹!" ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "|\n");
		printf(ANSI_COLOR_BOLD "\t\t+----------------------+\n");
		mostrar_stats_jugador(j_1);
	} else if (obtener_puntos_jugador(j_1) < obtener_puntos_jugador(j_2)) {
		printf(ANSI_COLOR_BOLD "\t\t+----------------------+\n");
		printf(ANSI_COLOR_BOLD
		       "\t\t|" ANSI_COLOR_CYAN ANSI_COLOR_BOLD
		       "¡Ganó el Jugador 2 👻!" ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "|\n");
		printf(ANSI_COLOR_BOLD "\t\t+----------------------+\n");
		mostrar_stats_jugador(j_2);

	} else {
		printf(ANSI_COLOR_BOLD "\t\t+-----------------------+\n");
		printf(ANSI_COLOR_BOLD
		       "\t\t|\t" ANSI_COLOR_BLUE ANSI_COLOR_BOLD
		       "!EMPATE! \t" ANSI_COLOR_RESET ANSI_COLOR_BOLD "|\n");
		printf(ANSI_COLOR_BOLD "\t\t+-----------------------+\n");

		printf("\n\n");

		printf(ANSI_COLOR_BOLD "\t\t+------------------------+\n");
		printf(ANSI_COLOR_BOLD
		       "\t\t|" ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       "¡Stats del Jugador 1 👹!" ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "|\n");
		printf(ANSI_COLOR_BOLD "\t\t+------------------------+\n");

		mostrar_stats_jugador(j_1);

		printf("\n");
		printf(ANSI_COLOR_BOLD "\t\t+------------------------+\n");
		printf(ANSI_COLOR_BOLD
		       "\t\t|" ANSI_COLOR_CYAN ANSI_COLOR_BOLD
		       "¡Stats del Jugador 2 👻!" ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       "|\n");
		printf(ANSI_COLOR_BOLD "\t\t+------------------------+\n");

		mostrar_stats_jugador(j_2);
	}
	printf("\n");
}
void jugar_juego(juego_t *j)
{
	if (!j)
		return;

	// escondo cursor y limpio pantalla antes de arrancar
	esconder_cursor();
	borrar_pantalla();

	// inicializo contexto
	logica_ctx_t ctx = { .j = j,
			     .jugador1_x = j->jugadores[0]->x,
			     .jugador1_y = j->jugadores[0]->y,
			     .jugador2_x = j->jugadores[1]->x,
			     .jugador2_y = j->jugadores[1]->y,
			     .juego_iniciado = true,
			     .graficos_mapa = GRAFICOS_V1 };
	// por defecto el mapa esta con graficos V1 si quiere cambiar los graficos remplazelo por GRAFICOS_V2

	// arranco el bucle: llamará a `tick` unas ~5 veces por segundo o al detectar tecla
	game_loop(logica_callback, &ctx);
	mostrar_resultados(ctx.j->jugadores[0], ctx.j->jugadores[1]);
	printf(ANSI_COLOR_BOLD "\t\t _______________________________\n");
	printf(ANSI_COLOR_BOLD
	       "\t\t|presiona " ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
	       "enter " ANSI_COLOR_RESET ANSI_COLOR_BOLD " para continuar|\n");
	printf(ANSI_COLOR_BOLD "\t\t ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");

	// al salir, restauro cursor y muestro ganador
	mostrar_cursor();
}

int main_juego(pokedex_t *p, int *semilla_elegida)
{
	int semilla;
	if (semilla_elegida == NULL) {
		semilla = (int)time(NULL);
	} else {
		semilla = *semilla_elegida;
	}
	if (p == NULL) {
		printf("no se pudo cargar los datos al juego\n");
		printf(ANSI_COLOR_BOLD
		       "\t\t _______________________________\n");
		printf(ANSI_COLOR_BOLD
		       "\t\t|presiona " ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
		       "enter " ANSI_COLOR_RESET ANSI_COLOR_BOLD
		       " para continuar|\n");
		printf(ANSI_COLOR_BOLD
		       "\t\t ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");

		return -1;
	}
	srand((unsigned int)semilla);
	juego_t *j = crear_juego(semilla);
	cargar_datos_al_juego(j, p);
	jugar_juego(j);
	destruir_juego(j);
	return 0;
}
