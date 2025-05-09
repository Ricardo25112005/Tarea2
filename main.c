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

//codigo para ejecutar el programa.    gcc -o tarea2 main.c TDAS/List.c TDAS/Map.c TDAS/Extra.c 
typedef struct {
  char id[50];           
  List *artists;
  char album_name[201];  
  char track_name[201];  
  int tempo;           
  char track_genre[101];  
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
  puts("     Base de Datos de Canciones");
  puts("========================================");

  puts("1) Cargar Canciones");
  puts("2) Buscar por id");
  puts("3) Buscar por autor");
  puts("4) Buscar por género");
  puts("5) Buscar por Tempo");
  puts("6) Salir");
}

void cargar_Canciones(HashMap *canciones_id, HashMap *canciones_genres, HashMap *canciones_artist, List *lista_lentas, List *lista_moderadas, List *lista_rapidas) {
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
    
    tipoCancion *Cancion = (tipoCancion *)malloc(sizeof(tipoCancion));
    strcpy(Cancion->id, campos[1]);       
    strcpy(Cancion->album_name, campos[3]);     
    strcpy(Cancion->track_name, campos[4]); 
    strcpy(Cancion->track_genre, campos[20]); 
    Cancion->artists = split_string(campos[2], ";");       
    Cancion->tempo = atoi(campos[18]); 

    insertMap(canciones_id, Cancion->id, Cancion);

    
    // Obtiener el genero de la canción
    Pair *genre_pair = searchMap(canciones_genres, Cancion->track_genre);
    if (genre_pair == NULL) {
      List *new_list = list_create();
      list_pushBack(new_list, Cancion);
      insertMap(canciones_genres, Cancion->track_genre, new_list);
    }
    else {
      // Si el género ya existe en el mapa, obtén la lista y agrega la película
      List *genre_list = (List *)genre_pair->value;
      list_pushBack(genre_list, Cancion);
    }

    char *artist = list_first(Cancion->artists);
    /*if (artist == NULL) {
      puts("error al leer artistas"); // Si no hay artistas, salta al siguiente
    }*/
    
    while (artist != NULL) {
        
        Pair *artist_pair = searchMap(canciones_artist, artist);
        if (artist_pair == NULL) {
            List *nueva_list = list_create();
            list_pushBack(nueva_list, Cancion);
            insertMap(canciones_artist, artist, nueva_list);
        } else {
            List *artist_list = (List *)artist_pair->value;
            list_pushBack(artist_list, Cancion);
        }

        
        artist = list_next(Cancion->artists);
    }

    if (Cancion->tempo < 60) {
      list_pushBack(lista_lentas, Cancion); // Agrega a la lista de lentas
    } else if (Cancion->tempo >= 60 && Cancion->tempo <= 120) {
      list_pushBack(lista_moderadas, Cancion); // Agrega a la lista de moderadas
    } else {
      list_pushBack(lista_rapidas, Cancion); // Agrega a la lista de rápidas
    }
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas


  // Itera sobre el mapa para mostrar las películas cargadas
  /*Pair *pair = firstMap(canciones_id);
  while (pair != NULL) {
    tipoCancion *cancion = pair->value;
    printf("ID: %s, Albun: %s, Titulo: %s, Tempo: %d\n", cancion->id, cancion->album_name, cancion->track_name, cancion->tempo);
    printf("Artista(s): ");
    for(char *artista = (char *) list_firts(cancion->artists); artista != NULL; artista = list_next(cancion->artists)){
      printf("%s    ", artista);
    }
    printf("\n");
    
    pair = nextMap(canciones_id); // Avanza al siguiente par en el mapa
  }*/
  puts("Se han cargado las canciones correctamente.");
}

void buscar_id(HashMap *canciones_id) {
  char id[50];
  printf("Ingrese el ID de la canción: ");
  scanf("%s", id);
  Pair *pair = searchMap(canciones_id, id);
  if (pair != NULL) {
    tipoCancion *cancion = pair->value;
    printf("ID: %s, Albun: %s, Titulo: %s, Tempo: %d\n", cancion->id, cancion->album_name, cancion->track_name, cancion->tempo);
    printf("Artista(s): ");
    for(char *artista = (char *) list_first(cancion->artists); artista != NULL; artista = list_next(cancion->artists)){
      printf("%s    ", artista);
    }
    printf("\n");
  } else {
    puts("No se encontró ninguna canción con ese ID.");
  }
}

void buscar_genero(HashMap *canciones_genres) {
  char genero[50];
  printf("Ingrese el género de la canción: ");
  scanf("%s", genero);
  puts("Canciones encontradas:");
  Pair *pair = searchMap(canciones_genres, genero);
  if (pair != NULL) {
    List *lista = (List *)pair->value;
    tipoCancion *cancion = list_first(lista);
    while (cancion != NULL) {
      printf("ID: %s, Albun: %s, Titulo: %s, Tempo: %d\n", cancion->id, cancion->album_name, cancion->track_name, cancion->tempo);
      printf("Artista(s): ");
      for(char *artista = (char *) list_first(cancion->artists); artista != NULL; artista = list_next(cancion->artists)){
        printf("%s    ", artista);
      }
      printf("\n");
      cancion = list_next(lista); // Avanza al siguiente elemento en la lista
    }
  } else {
    puts("No se encontró ninguna canción con ese género.");
  }
}

void buscar_artista(HashMap *canciones_artist) {
  char artista[50];
  printf("Ingrese el artista: ");
  scanf("%s[^/n]", artista);
  puts("Canciones encontradas:");
  Pair *pair = searchMap(canciones_artist, artista);
  if (pair != NULL) {
    List *lista = (List *)pair->value;
    tipoCancion *cancion = list_first(lista);
    while (cancion != NULL) {
      printf("ID: %s, Albun: %s, Titulo: %s, Tempo: %d\n", cancion->id, cancion->album_name, cancion->track_name, cancion->tempo);
      printf("Artista(s): ");
      for(char *artista = (char *) list_first(cancion->artists); artista != NULL; artista = list_next(cancion->artists)){
        printf("%s    ", artista);
      }
      cancion = list_next(lista); // Avanza al siguiente elemento en la lista
    }
  } else {
    puts("No se encontró ninguna canción de ese artista.");
  }
}

void buscar_tempo(List *lista_lentas, List *lista_moderadas, List *lista_rapidas) {
  limpiarPantalla();
  int tempo_deseado;
  puts("========================================");
  printf("Velocidad de tempo deseada(1 = lenta, 2 = moderada, 3 = rapida): ");
  scanf("%d", &tempo_deseado);
  if (tempo_deseado < 1 || tempo_deseado > 3) {
    puts("Opción no válida. Por favor, elija entre 1 y 3.");
    return;
  }
  if (tempo_deseado == 1) {
    puts("Canciones lentas:");
    tipoCancion *cancion = list_first(lista_lentas);
    while (cancion != NULL) {
      printf("ID: %s, Albun: %s, Titulo: %s, Tempo: %d\n", cancion->id, cancion->album_name, cancion->track_name, cancion->tempo);
      printf("Artista(s): ");
      for(char *artista = (char *) list_first(cancion->artists); artista != NULL; artista = list_next(cancion->artists)){
        printf("%s    ", artista);
      }
      printf("\n");
      cancion = list_next(lista_lentas); // Avanza al siguiente elemento en la lista
    }
  } else if (tempo_deseado == 2) {
    puts("Canciones moderadas:");
    tipoCancion *cancion = list_first(lista_moderadas);
    while (cancion != NULL) {
      printf("ID: %s, Albun: %s, Titulo: %s, Tempo: %d\n", cancion->id, cancion->album_name, cancion->track_name, cancion->tempo);
      printf("Artista(s): ");
      for(char *artista = (char *) list_first(cancion->artists); artista != NULL; artista = list_next(cancion->artists)){
        printf("%s    ", artista);
      }
      printf("\n");
      cancion = list_next(lista_moderadas); // Avanza al siguiente elemento en la lista
    }

  } else {
    puts("Canciones rápidas:");
    tipoCancion *cancion = list_first(lista_rapidas);
    while (cancion != NULL) {
      printf("ID: %s, Albun: %s, Titulo: %s, Tempo: %d\n", cancion->id, cancion->album_name, cancion->track_name, cancion->tempo);
      printf("Artista(s): ");
      for(char *artista = (char *) list_first(cancion->artists); artista != NULL; artista = list_next(cancion->artists)){
        printf("%s    ", artista);
      }
      printf("\n");
      cancion = list_next(lista_rapidas); // Avanza al siguiente elemento en la lista
    }
  }
}

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
  // (sin uso en este fragmento)

  HashMap *canciones_id = createMap(160000);
  HashMap *canciones_genres = createMap(160000);
  HashMap *canciones_artist = createMap(160000);
  List *lista_lentas = list_create(); // Lista para almacenar canciones lentas
  List *lista_moderadas = list_create(); // Lista para almacenar canciones moderadas
  List *lista_rapidas = list_create(); // Lista para almacenar canciones rápidas

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
      buscar_id(canciones_id);
      break;
    case '3':
      buscar_artista(canciones_artist);
      break;
    case '4':
      buscar_genero(canciones_genres);
      break;
    case '5':
      buscar_tempo(lista_lentas, lista_moderadas, lista_rapidas);
      break;
    }
  presioneTeclaParaContinuar();
  } while (opcion != '6');
  
  // Libera la memoria utilizada por los mapas
  map_clean(canciones_id);
  map_clean(canciones_genres);
  //map_clean(canciones_artist);
  list_clean(lista_lentas);
  list_clean(lista_moderadas);
  list_clean(lista_rapidas);
  limpiarPantalla();
  puts("Gracias por usar el programa. ¡Hasta luego!");
  return 0;
}
  