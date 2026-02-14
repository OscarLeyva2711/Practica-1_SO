#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <funciones.h>


typedef struct Pokemon {
    char apodo[50];
    char especie[50];
    int nivel;
    int xp;
    int clave;
} pokemon_t;


int main(){
    int opcion;
    do{
        printf("Pokedex\n");
        printf("1. Capturar Pokemon\n");
        printf("2. Mostrar Pokemones\n");
        printf("3. Eliminar Pokemon\n");
        printf("4. Salir\n");
        printf("Selecciona una accion:  \n");
        scanf("%d", &opcion);
        switch (opcion){
            case 1:
            // capturarPokemon();
            break;
            case 2: 
            // mostrarPokemon();
            break;
            case 3:
            // eliminarPokemon();
            break;
            case 4:
            printf("Adios\n");
            break;
            default: 
            printf("Selecciona una opcion valida \n"); 
        }
    }while (opcion !=4);
        return 0;
}

