/*
 * funciones.c
 * Contiene el cuerpo de todas las funciones declaradas en funciones.h
 * Las operaciones de archivo utilizan exclusivamente system calls de Linux:
 * open, read, write y close (sin fopen, fread, fwrite ni fclose).
 */

#include "funciones.h"

/* =========================================================
   FUNCIONES GENÉRICAS DE LISTA
   ========================================================= */

/*
 * insertar_nodo
 * Caso base: si *inicio es NULL, se crea el nodo aqui y se asigna
 * a *inicio. Gracias al doble puntero, este cambio se refleja
 * en la variable original del llamador.
 * Caso recursivo: avanza hacia el siguiente nodo hasta llegar al final.
 */
void insertar_nodo(Nodo **inicio, Pokemon dato) {
    if (*inicio == NULL) {
        Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
        nuevo->dato = dato;   
        nuevo->siguiente = NULL;  
        *inicio = nuevo;           
        return;
    }

    insertar_nodo(&((*inicio)->siguiente), dato);
}

/*
 * eliminar_nodo
 * Caso base 1: lista vacia, no hay nada que eliminar.
 * Caso base 2: el nodo actual es el que se busca. Se reasigna
 *              *inicio al siguiente y se libera la memoria.
 * Caso recursivo: avanza hasta encontrar el nodo con la clave.
 */
void eliminar_nodo(Nodo **inicio, int clave) {
    if (*inicio == NULL) {
        return; 
    }
    if ((*inicio)->dato.clave == clave) {
        Nodo *temp = *inicio;              
        *inicio = (*inicio)->siguiente;    
        free(temp);                        
        return;
    }
   
    eliminar_nodo(&((*inicio)->siguiente), clave);
}

/*
 * mostrar_lista
 * Recorre la lista recursivamente. En cada llamada imprime el nodo
 * actual usando el callback recibido, luego avanza al siguiente.
 * Caso base: lista vacia o fin de lista.
 */
void mostrar_lista(Nodo *inicio, void (*imprimir)(Pokemon)) {
    if (inicio == NULL) {
        return; /* Fin de la lista */
    }
    imprimir(inicio->dato);                        /* Imprime el nodo actual  */
    mostrar_lista(inicio->siguiente, imprimir);    /* Avanza al siguiente     */
}

/*
 * liberar_lista
 * Libera la memoria de forma recursiva llegando primero al ultimo
 * nodo y liberando en el camino de regreso.
 * Al final deja *inicio en NULL para evitar punteros colgantes.
 */
void liberar_lista(Nodo **inicio) {
    if (*inicio == NULL) {
        return; 
    }
    liberar_lista(&((*inicio)->siguiente)); 
    free(*inicio);                          
    *inicio = NULL;                        
}

/*
 * clave_existe
 * Recorre la lista recursivamente buscando la clave.
 * Retorna 1 en cuanto la encuentra, 0 si llega al final sin exito.
 */
int clave_existe(Nodo *inicio, int clave) {
    if (inicio == NULL) {
        return 0; 
    }
    if (inicio->dato.clave == clave) {
        return 1; 
    }
    return clave_existe(inicio->siguiente, clave); 
}

/* =========================================================
   FUNCIONES DE LA POKEDEX
   ========================================================= */

/*
 * imprimir_Pokemon
 * Callback que se pasa a mostrar_lista. Imprime todos los campos
 * de una Pokemon con un formato legible en terminal.
 */
void imprimir_Pokemon(Pokemon c) {
    printf("----------------------------------\n");
    printf("Clave      : %d\n", c.clave);
    printf("Especie    : %s\n", c.especie);
    printf("Apodo      : %s\n", c.apodo);
    printf("Nivel      : %d\n", c.nivel);
    printf("Experiencia: %d\n", c.experiencia);
    printf("----------------------------------\n");
}

/*
 * cargar_archivo_en_lista
 * Abre el archivo en modo solo lectura con open().
 * Si el archivo no existe (fd == -1), simplemente retorna.
 * Lee una Pokemon a la vez con read() hasta que no haya mas datos.
 * Cada Pokemon leida se inserta en la lista con insertar_nodo().
 * Al terminar cierra el file descriptor con close().
 */
void cargar_archivo_en_lista(Nodo **inicio) {
    // open() devuelve un file descriptor (entero) o -1 si falla 
    int fd = open(NOMBRE_ARCHIVO, O_RDONLY);
    if (fd == -1) {
        return; // El archivo aun no existe, no es un error critico 
    }

    Pokemon temp;
    /*
     * read() intenta leer sizeof(Pokemon) bytes del archivo y los
     * coloca en &temp. Devuelve cuantos bytes leyo realmente.
     * Si devuelve menos que sizeof(Pokemon), llegamos al final.
     */
    while (read(fd, &temp, sizeof(Pokemon)) == sizeof(Pokemon)) {
        insertar_nodo(inicio, temp);
    }

    close(fd); /* Libera el file descriptor */
}

/*
 * guardar_lista_en_archivo
 * Abre el archivo con O_WRONLY (Solo escritura), O_CREAT (De no existir el archivo, lo crea)
 *  y O_TRUNC (Si el archivo ya existe, lo vacia para escribirlo de nuevo):
 * El tercer parametro 0644 le da permisos de lectura y escritura unicamente al dueño del 
 * archivo.
 * Recorre la lista y escribe cada struct Pokemon con write().
 */
void guardar_lista_en_archivo(Nodo *inicio) {
    int fd = open(NOMBRE_ARCHIVO, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        printf("Error al abrir el archivo para escritura.\n");
        return;
    }

    Nodo *actual = inicio;
    while (actual != NULL) {
        /*
         * write() toma sizeof(Pokemon) bytes desde &actual->dato
         * y los escribe directamente en el archivo como datos binarios.
         */
        write(fd, &(actual->dato), sizeof(Pokemon));
        actual = actual->siguiente;
    }

    close(fd);
}

/*
 * capturar_Pokemon
 * 1. Carga el archivo en una lista temporal para verificar unicidad.
 * 2. Pide la clave y verifica que no exista en la lista.
 * 3. Pide el resto de los datos de la Pokemon.
 * 4. Abre el archivo con O_APPEND para agregar al final sin borrar
 *    lo existente y escribe la nueva Pokemon con write().
 * 5. Libera la lista temporal.
 */
void capturar_Pokemon() {
    Pokemon nueva;
    Nodo *lista_temp = NULL;
    char buffer[100]; 

    // Espejo del archivo en lista para validar clave unica 
    cargar_archivo_en_lista(&lista_temp);

    printf("\n=== CAPTURAR POKEMON ===\n");
    bool clave_valida = false;
    do{
        printf("Clave unica: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Elimina el \n del final 

        bool noHayLetras = true;
        for (int i =0; buffer[i] != '\0'; i++){
            if (buffer[i] < '0' || buffer[i] > '9'){
                noHayLetras = false;
                break;
            }
        }
        
        if(!noHayLetras){
            printf("La clave solo puede tener numeros, ingresa una clave valida.\n");
            continue;
        }
        nueva.clave = atoi(buffer); // Convierte el string a entero 
       
        if (clave_existe(lista_temp, nueva.clave)) {
            printf("Error: ya existe un Pokemon con esa clave. Intente otra.\n");
        } else {
            clave_valida = true;
        }
    } while (!clave_valida);
   
    

    printf("Especie    : ");
    fgets(nueva.especie, sizeof(nueva.especie), stdin);
    nueva.especie[strcspn(nueva.especie, "\n")] = '\0'; 

    printf("Apodo      : ");
    fgets(nueva.apodo, sizeof(nueva.apodo), stdin);
    nueva.apodo[strcspn(nueva.apodo, "\n")] = '\0';

    bool nivelValido = false;
    do {

    printf("Nivel      : ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    if (buffer[0] == '\0'){
        printf("Ingresa un nivel valido.\n");
        continue;
    }

    bool noHayLetrasNivel = true;
    for (int i =0; buffer[i] != '\0'; i++){
        if (buffer[i] < '0' || buffer[i] > '9'){
            noHayLetrasNivel = false;
            break;
        }
    }

    if (!noHayLetrasNivel) {
        printf("El nivel debe contener solo numeros.\n");
        continue;
    }

    nueva.nivel = atoi(buffer);
    nivelValido = true;
    } while (!nivelValido);

    bool expValida = false;
    do {

    printf("Experiencia: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (buffer[0] == '\0'){
        printf("Ingresa una experiencia valida.\n");
        continue;
    }
    bool noHayLetrasExp = true;
    for (int i =0; buffer[i] != '\0'; i++){
        if (buffer[i] < '0' || buffer[i] > '9'){
            noHayLetrasExp = false;
            break;
        }
    }
    if (!noHayLetrasExp) {
        printf("La experiencia debe contener solo numeros.\n");
        continue;
    }
    nueva.experiencia = atoi(buffer);
    expValida = true;
    } while (!expValida);

    /*
     * O_APPEND posiciona el cursor al final del archivo antes de
     * cada write(), garantizando que no se sobreescriba contenido.
     * O_CREAT crea el archivo si es la primera Pokemon capturada.
     */
    int fd = open(NOMBRE_ARCHIVO, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        printf("Error al abrir el archivo.\n");
        liberar_lista(&lista_temp);
        return;
    }

    /* Escribe la estructura completa en binario */
    write(fd, &nueva, sizeof(Pokemon));
    close(fd);

    printf("Pokemon capturada exitosamente.\n");

    liberar_lista(&lista_temp);
}

/*
 * mostrar_Pokemons
 * 1. Carga el archivo en la lista (espejo).
 * 2. Si la lista esta vacia, informa al usuario.
 * 3. Si tiene datos, recorre la lista con mostrar_lista()
 *    pasando imprimir_Pokemon como callback.
 * 4. Libera la lista al terminar.
 */
void mostrar_Pokemones() {
    Nodo *lista = NULL;
    cargar_archivo_en_lista(&lista); /* Carga el archivo en memoria */

    printf("\n=== Pokemones Resgistrados ===\n");

    if (lista == NULL) {
        printf("No hay Pokemones registrados.\n");
    } else {
        mostrar_lista(lista, imprimir_Pokemon);
    }

    liberar_lista(&lista); 
}

/*
 * eliminar_Pokemon
 * 1. Pide la clave de la Pokemon a eliminar.
 * 2. Carga el archivo en la lista (espejo).
 * 3. Verifica que la clave exista en la lista.
 * 4. Elimina el nodo correspondiente de la lista.
 * 5. Sobreescribe el archivo con la lista ya sin esa Pokemon.
 * 6. Libera la lista al terminar.
 */
void eliminar_Pokemon() {
    int clave;
    printf("\n=== ELIMINAR Pokemon DIGITAL ===\n");
    printf("Ingrese la clave de la Pokemon a eliminar: ");
    scanf("%d", &clave);
    getchar();

    Nodo *lista = NULL;
    cargar_archivo_en_lista(&lista); 

    if (!clave_existe(lista, clave)) {
        printf("No se encontro ninguna Pokemon con esa clave.\n");
        liberar_lista(&lista);
        return;
    }

    eliminar_nodo(&lista, clave);        
    guardar_lista_en_archivo(lista);             

    printf("Pokemon eliminada exitosamente.\n");

    liberar_lista(&lista);
}