#ifndef FUNCIONES_H
#define FUNCIONES_H


#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     
#include <fcntl.h>      /* Libreria para Syscalls     */
#include <unistd.h>     /* Libreria para Syscalls     */
#include <stdbool.h>    
/* =========================================================
   ESTRUCTURA DE DATOS
   ========================================================= */

typedef struct Pokemon {
    char especie[50];
    char apodo[50];
    int  clave;
    int  nivel;
    int  experiencia;
} Pokemon;

typedef struct Nodo {
    Pokemon dato;
    struct Nodo *siguiente;
} Nodo;

/* =========================================================
   FUNCIONES GENERICAS DE LISTA
   ========================================================= */

/*
 * insertar_nodo
 * Inserta un nuevo nodo al final de la lista de forma recursiva.
 * Recibe doble puntero para poder modificar el inicio si la lista está vacía.
 */
void insertar_nodo(Nodo **inicio, Pokemon dato);

/*
 * eliminar_nodo
 * Elimina de la lista el nodo cuya clave coincida con la indicada.
 * Usa doble puntero para poder reasignar el inicio si el nodo a eliminar
 * es el primero. Opera de forma recursiva.
 */
void eliminar_nodo(Nodo **inicio, int clave);

/*
 * mostrar_lista
 * Recorre la lista de forma recursiva e imprime la informacion de cada nodo.
 * No depende del contexto: delega el formato de impresion a una funcion externa.
 */
void mostrar_lista(Nodo *inicio, void (*imprimir)(Pokemon));

/*
 * liberar_lista
 * Libera toda la memoria de la lista de forma recursiva.
 */
void liberar_lista(Nodo **inicio);

/*
 * clave_existe
 * Verifica recursivamente si una clave ya existe en la lista.
 * Retorna 1 si existe, 0 si no.
 */
int clave_existe(Nodo *inicio, int clave);

/* =========================================================
   FUNCIONES PARA LA GESTION DE LA POKEDEX
   ========================================================= */

#define NOMBRE_ARCHIVO "Pokemones.dat"

/*
 * imprimir_Pokemon
 */
void imprimir_Pokemon(Pokemon c);

/*
 * cargar_archivo_en_lista
 * Abre el archivo binario y copia cada Pokemon en la lista.
 * Refleja el contenido del archivo en la lista (espejo).
 */
void cargar_archivo_en_lista(Nodo **inicio);

/*
 * guardar_lista_en_archivo
 * Sobreescribe el archivo con el contenido actual de la lista.
 * Se usa despues de eliminar una Pokemon.
 */
void guardar_lista_en_archivo(Nodo *inicio);

/*
 * capturar_Pokemon
 * Solicita los datos de una nueva Pokemon al usuario.
 * Verifica que la clave sea unica cargando el archivo en una lista temporal.
 * Agrega la nueva Pokemon al final del archivo.
 */
void capturar_Pokemon();

/*
 * mostrar_Pokemons
 * Carga el archivo en la lista y recorre la lista para mostrar cada Pokemon.
 * Libera la lista al finalizar.
 */
void mostrar_Pokemones();

/*
 * eliminar_Pokemon
 * Carga el archivo en la lista, elimina el nodo con la clave indicada,
 * y sobreescribe el archivo con la lista resultante.
 * Libera la lista al finalizar.
 */
void eliminar_Pokemon();

#endif /* FUNCIONES_H */