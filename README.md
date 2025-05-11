Base de datos de canciones
=====
---
Para esta Programa de base de datos de canciones se implementaran tres **Mapa** para organizar las canciones en Id, por artista y por genero con un mapa por cada una de estas categorias, ademas de implementar tres **Listas**  para organizar cada cancion segun su tempo lento, moderado y rapido con una lista por cada una de estas categorias, esto utilizando las siguiente estructura:
````c
typedef struct Node Node;
typedef struct List List;

struct Node{
    void * ticket; 
    Node * next;
    Node * prev;
};

struct List{
    Node * head;
    Node * tail;
    Node * current;
};
````
````c
typedef struct HashMap HashMap;
typedef struct Pair Pair;

typedef struct Pair {
     char * key;
     void * value;
} Pair;

struct HashMap {
    Pair ** buckets;
    long size;
    long capacity;
    long current;
};
````
para cada canción se utiliza el siguiente struct:
````c
typedef struct {
  char id[50];           
  List *artists;
  char album_name[201];  
  char track_name[201];  
  int tempo;           
  char track_genre[101];  
} tipoCancion;
````
---
Este programa tiene como finalidad crear tres mapas donde se almacenaran las canciones, el primer mapa es por id utilizando el ID como Key, el segundo mapa es por genero el cual utiliza el genero como Key y para cada genero se tiene una lista con las canciones que tienen este genero y el ultimo mapa es por artista, utilizando como Key el nombre del artista, para cada artista se tiene una lista con las canciones del artista. Almacena las canciones en una de las tres listas de tempo (Lentas, moderadas o rapidas), el prgrama tiene la capacidad de buscar por ID, genero, artista y por tempo mostrando las caracteristicas de cada cancion encontrada. El programa se divide en 5 secciones:

1. Cargado de canciones: En esta parte se llama a la funcion `void cargar_Canciones(HashMap *canciones_id, HashMap *canciones_genres, HashMap *canciones_artist, List *lista_lentas, List *lista_moderadas, List *lista_rapidas)`, funcion la cual recive los tres mapas y las tres listas, el programa abre el archivo csv llamado song_dataste_.csv, lee cada linea del csv y almacena las variables dentro de un auxiliar, luego almacena la cancion dentro del mapa de canciones por ID utilizando la funcion `insertMap`, luego obtiene el genero de la cancion y la busca dentro del mapa de generos, en caso de que la cancion no este en el mapa, crea la lista de canciones agrega la cancion a esta lista y agrega al mapa el genero y la lista, en caso de existir el genero en la lista se obtiene la lista de canciones y agrega la cancion a la lista, luego repite el mismo proceso del mapa por genero para el mapa de artistas, con la diferencia que para el mapa de artistas se hace este proceso para cada artista de la cancion, luego agrega la cancion en una de las tres listas enlazadas por tempo (lentas, moderadas, rapidas), para finalmente mostrar un mensaje de que se cargaron la canciones. 

2. Buscar por ID: En esta seccion la funcion a llamar es `void buscar_id(HashMap *canciones_id)` la cual lee el ID que deseamos buscar, busca este ID en el mapa de canciones por ID, en caso de encontrarlo muestra en pantalla todas las caracteristicas de la cancion, si la cancion no es encontrada se muestra un mensaje en pamtalla indicando que la cancion no fue encontrada.

3. Mostrar canciones del artista: en esta parte se llama a la funcion `void buscar_artista(HashMap *canciones_artist)` la cual lee el nombre del artista al cual le queremos buscar sus canciones, busca en el mapa de artista si se encuentra el artista, en caso de encontrar el artista, se muestra en pantalla la lista con todas las canciones del artista y las canciones con sus respectivas caracteristicas, si no se encuentra el artista se muestra un mensaje indicando que no hay canciones de ese artista.

4. Mostrar canciones del genero: se llama a la funcion `void buscar_genero(HashMap *canciones_genres)` la cual lee el genero el cual queremos buscar, buscando si se encuentra el genero en el mapa de canciones por genero, en caso de encontrar el genro se mustra la lista de las canciones del genero con sus respectivas caracteristicas, si no se encuentra el genero se muestra en pantalla un mensaje indicando que no se encontraron canciones con ese genero.

5. Mostrar canciones por tempo: en esta parte se llama a la funcion `void buscar_tempo(List *lista_lentas, List *lista_moderadas, List *lista_rapidas)` la cual muestra lee el tempo deseado en formato de numero (1 para lentas, 2 para moderadas y 3 para rapidas) y muestra la lista de canciones con ese tempo, mostrando las caracteristicas de cada cancion, en caso de ingresar un numero no relacionado relacionados a las listas se muestra un mensaje indicando que la opcion no es valida y regresa al menu principal.
   
Luego para compilar y ejecutar:

    gcc -o tarea2 main.c TDAS/List.c TDAS/Map.c TDAS/Extra.c
    ./tarea2 
