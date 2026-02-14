#ifndef funciones_H
#define funciones_H

typedef struct nodo{
    int info;
    struct nodo *sig;
} nodo_t;
void insertar_inicio(nodo_t**inicio);
#endif