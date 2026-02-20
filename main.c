#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"


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
        getchar();
        switch (opcion){
            case 1:
            capturar_Pokemon();
            break;
            case 2:
            mostrar_Pokemones();
            break;
            case 3:
            eliminar_Pokemon();
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

