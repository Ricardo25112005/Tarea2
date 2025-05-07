#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "TDAS/List.h"
#include "TDAS/Map.h"
#include "TDAS/Extra.h"
#define MAX_LINE 

//codigo para ejecutar el programa.    gcc -o tarea1 main.c list.c 
typedef struct {
  char id[50];           
  List *artists;
  char album_name[121];  
  char track_name[121];  
  int tempo;           
  char track_genre[50];  
} tipoCancion;

void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
    puts("Presione una tecla para continuar...");
    getchar(); // Consume el '\n' del buffer de entrada
    getchar(); // Espera a que el usuario presione una tecla
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Canciones");
  puts("2) Buscar por id");
  puts("3) Buscar por autor");
  puts("4) Buscar por género");
  puts("5) Buscar por Tempo");
  puts("6) Salir");
}

void cargar_Canciones(HashMap *canciones_id, HashMap *canciones_genres, HashMap *canciones_artist, List *lista_lentas, List *lista_moderadas, List *lista_rapidas) {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("Data/song_dataset_.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV
  //unsigned long long k = 0;
  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    //k++;
    //if (k == 1000) break; // Salta la primera línea (encabezados)
    // Crea una nueva estructura Film y almacena los datos de cada película
    tipoCancion *Cancion = (tipoCancion *)malloc(sizeof(tipoCancion));
    strcpy(Cancion->id, campos[1]);        // Asigna ID
    strcpy(Cancion->album_name, campos[3]);     // Asigna título
    strcpy(Cancion->track_name, campos[4]); // Asigna director
    strcpy(Cancion->track_genre, campos[20]); // Asigna director
    Cancion->artists = split_string(campos[2], ";");       // Inicializa la lista de géneros
    Cancion->tempo = atoi(campos[18]); // Asigna año, convirtiendo de cadena a entero

    
    // Inserta la película en el mapa usando el ID como clave
    insertMap(canciones_id, Cancion->id, Cancion);

    // Código generado con ayuda de chatgpt3.5
    // conversación: https://chat.openai.com/share/5f0643ad-e8f5-4fb7-a0fa-2d2f92408429
    
    // Obtiener el genero de la canción
    Pair *genre_pair = searchMap(canciones_genres, Cancion->track_genre);
    if (genre_pair == NULL) {
      List *new_list = create_List();
      push_back(new_list, Cancion);
      insertMap(canciones_genres, Cancion->track_genre, new_list);
    }
    else {
      // Si el género ya existe en el mapa, obtén la lista y agrega la película
      List *genre_list = (List *)genre_pair->value;
      push_back(genre_list, Cancion);
    }

    char *artist = list_firts(Cancion->artists);
    // Itera sobre cada género de la película
    while (artist != NULL) {
        // Busca el género en el mapa pelis_bygenres
        Pair *artist_pair = searchMap(canciones_artist, artist);

        // Si el género no existe en el mapa, crea una nueva lista y agrégala al mapa
        if (artist_pair == NULL) {
            List *nueva_list = create_List();
            push_back(nueva_list, Cancion);
            insertMap(canciones_artist, artist, nueva_list);
        } else {
            // Si el género ya existe en el mapa, obtén la lista y agrega la película
            List *artist_list = (List *)artist_pair->value;
            push_back(artist_list, Cancion);
        }

        // Avanza al siguiente género en la lista
        artist = list_next(Cancion->artists);
    }
    if (Cancion->tempo < 60) {
      push_back(lista_lentas, Cancion); // Agrega a la lista de lentas
    } else if (Cancion->tempo >= 60 && Cancion->tempo <= 120) {
      push_back(lista_moderadas, Cancion); // Agrega a la lista de moderadas
    } else {
      push_back(lista_rapidas, Cancion); // Agrega a la lista de rápidas
    }
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas


  // Itera sobre el mapa para mostrar las películas cargadas
  Pair *pair = firstMap(canciones_id);
  while (pair != NULL) {
    tipoCancion *cancion = pair->value;
    printf("ID: %s, Albun: %s, Titulo: %s, Tempo: %d\n", cancion->id, cancion->album_name, cancion->track_name, cancion->tempo);
    printf("Artista(s): ");
    for(char *artista = (char *) list_firts(cancion->artists); artista != NULL; artista = list_next(cancion->artists)){
      printf("%s    ", artista);
    }
    printf("\n");
    
    pair = nextMap(canciones_id); // Avanza al siguiente par en el mapa
  }
  puts("Se han cargado las canciones correctamente.");
}




int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
  // (sin uso en este fragmento)

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  HashMap *canciones_id = createMap(160000);
  HashMap *canciones_genres = createMap(80000);
  HashMap *canciones_artist = createMap(80000);
  List *lista_lentas = create_List(); // Lista para almacenar canciones lentas
  List *lista_moderadas = create_List(); // Lista para almacenar canciones moderadas
  List *lista_rapidas = create_List(); // Lista para almacenar canciones rápidas

  // Recuerda usar un mapa por criterio de búsqueda

  do {  
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_Canciones(canciones_id, canciones_genres, canciones_artist, lista_lentas, lista_moderadas, lista_rapidas);
      break;
    case '2':
      //buscar_id(canciones_id);
      break;
    case '3':
      break;
    case '4':
      //buscar_genero(canciones_genres);
      break;
    case '5':
      break;
    }
  presioneTeclaParaContinuar();
} while (opcion != '6');
  
  // Libera la memoria utilizada por los mapas
  //map_destroy(canciones_id);
  //map_destroy(canciones_genres);
  //map_destroy(canciones_artist);
  limpiarPantalla();
  puts("Gracias por usar el programa. ¡Hasta luego!");
return 0;
}
  