<p align="center"><a href="https://xanubot.a2m.com.ar/jobs/5314"><img src="https://xanubot.a2m.com.ar/badges/5314.svg" alt="Xanubot"></a>

<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Repositorio de Rodrigo Luciano Ururi - 112823 - rururi@fi.uba.ar

- Para compilar:

```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g src/*.c tp2_con_animaciones.c engine.o -o tp2
```

- Para ejecutar:

```bash
./tp2 archivo_csv
```

- Para ejecutar con valgrind:
```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes -s ./tp2 archivo_csv
```
---
# INFORME TP2
---
# TDAs CREADOS

---

## TDA Mapa

El **TDA Mapa** modela una estructura bidimensional de tamaño 12x12 que representa el entorno del juego. Cada celda del mapa puede contener múltiples elementos (como pokemones, jugadores etc.), lo que permite una interacción compleja y dinámica. Este TDA fue implementado utilizando el **TDA Lista**, aprovechando su capacidad para almacenar múltiples objetos heterogéneos en cada posición.

### Funcionalidades principales:

- `crear_mapa`: Inicializa un nuevo mapa vacío de 12x12 posiciones.
- `agregar_objeto_al_mapa`: Inserta un objeto genérico en una coordenada específica `(x, y)`.
- `posicion_vacia`: Verifica si una posición se encuentra vacía.
- `obtener_mapa`: Devuelve el objeto ubicado en `(x, y)` sin eliminarlo.
- `sacar_del_mapa`: Remueve y retorna el objeto de la posición `(x, y)`.
- `destruir_mapa`: Libera todos los recursos asociados al mapa.

---

## TDA Mapa Objetos

El **TDA Mapa Objetos** complementa al TDA Mapa, implementando la lógica del juego relacionada con las entidades principales: **jugadores** y **pokemones**. Este TDA encapsula las acciones posibles de cada tipo de objeto y su interacción con el mapa.

### Jugador

Representa al usuario dentro del juego, gestionando su posición, puntaje y lista de pokemones capturados.

#### Funcionalidades:

- `crear_jugador`: Crea un jugador con su posición inicial, puntaje y estructuras auxiliares.
- `mover_jugador`: Permite desplazar al jugador dentro del mapa, validando que la nueva posición sea válida.
- `destruir_jugador`: Libera todos los recursos asociados al jugador.

### Pokemon

Los **pokemones** son entidades que aparecen en el mapa y pueden ser capturados por los jugadores.

#### Funcionalidades:

- `crear_pokemon_juego`: Crea un nuevo pokemon con coordenadas generadas a partir de una semilla.
- `destruir_pokemon`: Libera la memoria asociada al pokemon.

### Captura e Interacción

Controla la lógica central del juego: la captura de pokemones y la relación entre jugadores y sus pilas de acumulación.

#### Funcionalidades:

- `capturar_pokemon`: Permite al jugador capturar un pokemon y sumarlo a su lista personal.
- `obtener_ultimo_capturado`: Devuelve el último pokemon que el jugador capturó.
- `agregar_acumulado`: Añade un pokemon a la pila de acumulación del rival.
- `tiene_acumulado`: Informa si el jugador tiene pokemones acumulados.
- `sacar_acumulado`: Extrae el tope de la pila de acumulación.
- `verificar_tipo_poke`: Verifica si el tipo del nuevo pokemon coincide con el tope de la pila.
- `sumar_puntos` / `restar_puntos`: Modifican el puntaje del jugador.
- `generar_nuevo_pokemon`: Crea un nuevo pokemon para ser insertado nuevamente en el mapa.

---

## TDA Juego Datos

El **TDA Juego Datos** funciona como núcleo centralizador de los datos globales del juego, integrando mapa, jugadores, pokemones, acumulados y temporizador. Esta estructura permite mantener el estado completo de la partida en un solo lugar.

### Funcionalidades:

- `crear_juego`: Inicializa todos los componentes del juego a partir de una semilla de aleatoriedad.
- `cargar_datos_al_juego`: Carga los pokemones desde la pokedex al mapa y a la lista de disponibles.
- `cargar_pokemones_al_mapa`: Inserta los pokemones iniciales en el mapa, distribuyéndolos aleatoriamente.
- `destruir_juego`: Libera todos los recursos utilizados durante la partida, incluyendo mapa, jugadores y pokemones.

---


## TDA Menu

El **TDA Menu** permite al usuario interactuar con el tipo de menu que haya creado previamente. a través de una interfaz dinámica compuesta por operaciones asignadas a teclas. Este TDA actúa como intermediario entre el usuario y la lógica del programa, facilitando el acceso a diferentes funcionalidades como "Jugar", "Buscar", "Mostrar", en el caso del trabajo final.

### Usos en el programa:

- **Interfaz de interacción principal**: El menú se muestra al usuario al iniciar el juego, permitiéndole seleccionar entre distintas operaciones disponibles.
- **Ejecución dinámica de funciones**: Cada operacion del menú está asociada a una función que se ejecuta al presionar la tecla correspondiente.

### Justificación de uso:

- **Estructura extensible y flexible**: Gracias al uso de un TDA Hash como implementacion, el menú puede buscar y ejecutar operaciones en tiempo constante promedio (`O(1)`), lo que asegura una respuesta inmediata a la interacción del usuario.
- **Asignación directa de comportamiento a teclas**: Las operaciones están asociadas a claves de teclado (como "J" para jugar o "S" para salir), lo cual facilita una interfaz clara e intuitiva.
- **Separación de responsabilidades**: El TDA encapsula toda la lógica relacionada con la interacción textual del usuario, evitando que se mezcle con la lógica interna del juego.
- **Iteración eficiente y visualización**: El menú puede listar todas las teclas y operaciones disponibles mediante la función `mostrar_operaciones`, que itera sobre el hash y permite al usuario conocer sus opciones en cada momento.
- **Destrucción controlada**: El menú y sus operaciones pueden ser destruidos de manera ordenada y segura con `menu_destruir`, liberando toda la memoria utilizada.

### Funciones principales:

- `crear_menu`: Inicializa un menú con un nombre identificador.
- `crear_operacion`: Crea una operación asociada a una función ejecutable.
- `menu_agregar_operacion`: Asocia una tecla a una operación.
- `menu_eliminar_operacion`: Elimina una operación existente.
- `cantidad_operaciones`: Devuelve la cantidad de operaciones disponibles.
- `iniciar_menu`: Muestra el menú e inicia la interacción con el usuario.
- `ejecutar_operacion`: Ejecuta la función asociada a una tecla específica.
- `mostrar_operaciones`: Lista en pantalla todas las teclas y operaciones activas.
- `destruir_operacion / destruir_menu`: Libera los recursos utilizados por el menú y sus operaciones.


# TDAs REUTILIZADOS

---

## TDA Lista

El **TDA Lista** fue reutilizado como estructura base en múltiples componentes del juego por su capacidad de **almacenar elementos de manera ordenada y dinámica**.

### Usos en el programa:

- **Modelado del mapa**: Cada casilla del mapa contiene una lista de objetos, lo que permite que múltiples entidades (pokemones, objetos) coexistan en una misma posición sin restricciones.
- **Gestión de pokemones disponibles**: Los pokemones disponibles durante la partida están almacenados en una lista, que se recorre y modifica dinámicamente a medida que los jugadores los capturan.
- **Pokemones capturados**: Cada jugador tiene su propia lista de pokemones capturados, que puede visualizar al final de la partida.

### Justificación de uso:

- **Inserción y eliminación dinámica**: Permite agregar o quitar elementos en cualquier posición de forma eficiente, ideal para el mapa o la lista de capturas que cambia constantemente.
- **Orden preservado**: A diferencia de otras estructuras como el hash, la lista mantiene el orden de inserción, útil para mostrar los pokemones capturados en el orden en que fueron obtenidos.
- **Recorrido secuencial simple**: Es fácil de recorrer de principio a fin, lo que simplifica el código en muchas funcionalidades del juego.
- **Interfaz uniforme**: Su interfaz sencilla permite reutilizarla en distintos contextos sin modificar su implementación.

---

## TDA Pila

El **TDA Pila** fue utilizado para implementar la mecánica de **pila de pokemones acumulados** que cada jugador va teniendo a lo largo de la partida.

### Usos en el programa:

- **Mecánica de captura estratégica**: Cada vez que un jugador captura un pokémon, este se **añade a la pila de su rival**, acumulando presión.
- **Pila de bonificación**: Si un jugador captura un pokémon del mismo tipo que el que se encuentra en el tope de su pila, este se **desapila** y se **aumenta un punto al jugador**.

### Justificación de uso:

- **Eficiencia en inserciones y extracciones**: Las operaciones de apilar y desapilar tienen costo constante O(1), ideal para el juego donde estas operaciones se realizan frecuentemente.
- **Simplicidad en la interfaz**: La pila tiene una interfaz acotada pero poderosa, lo que evita errores y hace el manejo de la lógica de acumulación más claro y seguro.
- **Representación visual directa**: La pila facilita mostrar al usuario el tope de su acumulación, haciendo evidente qué tipo de pokémon debería capturar estratégicamente.


---

## TDA Hash

El **TDA Hash** fue reutilizado como estructura principal para implementar el **TDA Menu**, permitiendo una gestión eficiente de las operaciones disponibles.

### Justificación de uso:

- **Almacenamiento de operaciones**: Cada operación del menú se almacena en una tabla hash, asociada a una tecla identificadora (clave) que el usuario puede presionar para ejecutarla. Esto permite búsquedas rápidas (O(1) promedio) para ejecutar la función correspondiente a la opción elegida.
- **Agregación y eliminación dinámica**: Gracias a la flexibilidad del hash, el menú puede agregar (`menu_agregar_operacion`) o eliminar (`menu_eliminar_operacion`) operaciones de forma eficiente durante la ejecución del programa.
- **Iteración eficiente de claves**: Mediante `hash_iterar_claves`, el menú puede listar todas las operaciones disponibles (`mostrar_operaciones`) de manera sencilla, sin necesidad de estructuras auxiliares adicionales.
- **Asociación entre clave y comportamiento**: El hash mapea teclas (como "J" para jugar o "S" para salir) directamente a funciones ejecutables, facilitando una interfaz dinámica y extensible.

## POKEDEX (TP_1)

El archivo correspondiente al **TP_1** se encarga de cargar la información de los pokemones desde un archivo `.csv`, funcionando como una **pokedex base** para el juego. Este componente procesa y almacena los datos necesarios de cada pokemon, como su nombre, tipo, fuerza, ID, entre otros atributos relevantes.

### Funcionalidades principales:

- **Lectura del archivo `.csv`**: Se parsea línea por línea el archivo que contiene la base de datos de pokemones.
- **Filtrado de datos válidos**: Se descartan registros incompletos o mal formateados.

Esta pokedex es fundamental para la configuración inicial del juego, ya que garantiza la existencia de un conjunto variado y válido de pokemones con los que interactuar.
