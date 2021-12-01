#include "hash.h"
#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define EXITO 0
#define ERROR -1

#define FACTOR_DE_CARGA 0.75
#define NULO 0


struct hash{
    lista_t** tabla;
    size_t capacidad;
    size_t cantidad_elementos;
    hash_destruir_dato_t destructor;
};


/**
 * PRE: Recibe una capacidad mayor o igual a 3.
 * POST: Reserva la memoria necesaria para una tabla(de tipo lista_t) del tamaño de la 
 * capacidad recibida. Luego reserva la memoria necesaria para cada lista de la tabla.
 * Devuelve la tabla con su memoria reservada o NULO en caso de error.
 */ 
lista_t** crear_tabla(size_t capacidad){
    lista_t** nueva = calloc(capacidad+1, sizeof(lista_t));
    if(!nueva){
        return NULO;
    }

    for(int i = 0; i < capacidad; i++){
        nueva[i] = lista_crear();
    }

    return nueva;
}


hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial){
    if(capacidad_inicial < 3){
        capacidad_inicial = 3;
    }

    hash_t* nuevo_hash = malloc(sizeof(hash_t));
    if(!nuevo_hash) return NULO;

    lista_t** nueva_tabla = crear_tabla(capacidad_inicial);
    if(!nueva_tabla){
        free(nuevo_hash);
        return NULO;
    }

    nuevo_hash->tabla = nueva_tabla;
    nuevo_hash->destructor = destruir_elemento;
    nuevo_hash->capacidad = capacidad_inicial;
    nuevo_hash->cantidad_elementos = 0;

    return nuevo_hash;    
}


/**
 * PRE: Recibe una clave diferente de NULL y una capacidad mayor o igual a 3.
 * POST: Devuelve la posición de la tabla hash en donde se debiera inserta el elemento 
 * con la clave recibida, dependiendo de la capacidad de la tabla.
 */ 
size_t funcion_hash(const char* clave, size_t capacidad){
    size_t posicion = 0;

    while(*clave){
        posicion += *(clave++);
    }
    return (posicion%capacidad);
}


/**
 * PRE: Recibe un hash válido.
 * POST: Reserva memoria para una nueva tabla según nueva_capacidad. Inserta en la nueva
 * tabla todos los elementos que estaban en la tabla vieja. Asigna la tabla nueva al hash y 
 * lo actualiza. Luego libera la memoria que ocupaba la tabla vieja.
 * Devuelve EXITO si se logró lo buscado, o ERROR en caso de error.
 */ 
int rehash(hash_t* hash, size_t nueva_capacidad){
    lista_t** tabla_nueva = crear_tabla(nueva_capacidad);
    if(!tabla_nueva) return ERROR;

    lista_t** tabla_vieja = hash->tabla;
    size_t capacidad_vieja = hash->capacidad;

    hash->tabla = tabla_nueva;
    hash->cantidad_elementos = 0;
    hash->capacidad = nueva_capacidad;

    size_t i = 0;
    lista_iterador_t* iterador;
    while(i < capacidad_vieja){
        if(tabla_vieja[i]->nodo_inicio){
            for(iterador = lista_iterador_crear(tabla_vieja[i]); lista_iterador_tiene_siguiente(iterador); lista_iterador_avanzar(iterador)){
                hash_insertar(hash, iterador->corriente->clave, iterador->corriente->elemento);
            }
            lista_iterador_destruir(iterador);
        }
        lista_destruir(tabla_vieja[i]);
        i++;
    }
    
    free(tabla_vieja);

    return EXITO;
}


/**
 * PRE: Recibe una clave diferente de NULL.
 * POST: Reserva la memoria necesaria para otra clave idéntica, luego copia en la 
 * dirección de memoria reservada la clave recibida. 
 * Devuelve el puntero a la dirección de memoria creada, o NULO en caso de error.
 */ 
void* crear_copia_clave(char* clave){
    void* copia = malloc(strlen(clave) + 1);
    if(!copia) return NULO;

    strcpy(copia, clave);

    return copia;
}


int hash_insertar(hash_t* hash, const char* clave, void* elemento){
    if( !hash || !clave ) return ERROR;
    
    
    if( ((hash->cantidad_elementos)/(hash->capacidad)) >= FACTOR_DE_CARGA ){
        rehash(hash, (hash->capacidad)*2);
    }


    size_t posicion_a_insertar = funcion_hash(clave, hash->capacidad);
    if(hash_contiene(hash, clave)){
        hash_quitar(hash, clave);
    }
    
    void* copia_clave = crear_copia_clave((char*)clave);
    lista_insertar(hash->tabla[posicion_a_insertar], elemento, copia_clave);
    (hash->cantidad_elementos)++;
    
    return EXITO;
}


/**
 * PRE: Recibe un hash válido, la posición en la tabla de hash en la que se encuentra el 
 * elemento buscado, y la clave asociada a ese elemento que es diferente de NULL.
 * POST: Se crea un iterador interno para recorrer la lista asociada a la posición de tabla recibida,
 * si se encuentra el elemento con la clave buscada, devuelve su posición en la lista(o el número
 * de iteraciones realizadas hasta encontrarlo).
 * En caso de que no se encuentre devuelve ERROR.
 */ 
size_t posicion_elemento_segun_clave(hash_t* hash, size_t posicion_en_tabla, const char* clave_buscada){
    bool encontrado = false;
    lista_iterador_t* iterador;
    
    size_t pos_elemento_buscado = ERROR;
    size_t iteraciones = 0;
    for(iterador = lista_iterador_crear(hash->tabla[posicion_en_tabla]); (lista_iterador_tiene_siguiente(iterador) && !encontrado); lista_iterador_avanzar(iterador)){
        if(strcmp(iterador->corriente->clave, clave_buscada) == 0){
            pos_elemento_buscado = iteraciones;
            encontrado = true;
        }
        iteraciones++;
    }
    lista_iterador_destruir(iterador);

    return pos_elemento_buscado;
}


int hash_quitar(hash_t* hash, const char* clave){
    if( !hash || !clave ) return ERROR;

    size_t posicion_en_tabla = funcion_hash(clave, hash->capacidad);

    size_t pos_elemento_buscado = posicion_elemento_segun_clave(hash, posicion_en_tabla, clave);
    if(pos_elemento_buscado == ERROR) return ERROR;
    
    
    void* elemento_quitado = lista_quitar_de_posicion(hash->tabla[posicion_en_tabla], pos_elemento_buscado);
    if(hash->destructor){
        hash->destructor(elemento_quitado);
    }

    (hash->cantidad_elementos)--;

    return EXITO;
}


void* hash_obtener(hash_t* hash, const char* clave){
    if( !hash || !clave ) return NULO;
    
    size_t posicion_en_tabla = funcion_hash(clave, hash->capacidad);

    size_t pos_elemento_buscado = posicion_elemento_segun_clave(hash, posicion_en_tabla, clave);
    if(pos_elemento_buscado == ERROR) return NULO;
    
    return (lista_elemento_en_posicion(hash->tabla[posicion_en_tabla], pos_elemento_buscado)); 
}


bool hash_contiene(hash_t* hash, const char* clave){
    if( !hash || !clave ) return NULO;

    return (hash_obtener(hash, clave));  
}


size_t hash_cantidad(hash_t* hash){
    if(!hash) return NULO;
    
    return (hash->cantidad_elementos);
}


/**
 * POST: Recorre recursivamente los nodos enlazados al nodo recibido. 
 * Le aplica el destructor a cada elemento y libera la memoria utilizada por cada nodo
 * y su clave.
 */ 
void destruir_lista(nodo_t* nodo_lista, hash_destruir_dato_t destructor){
    if(!nodo_lista) return;

    destruir_lista(nodo_lista->siguiente, destructor);

    if(destructor){
        destructor(nodo_lista->elemento);
    }
    free(nodo_lista->clave);
    free(nodo_lista);
}


void hash_destruir(hash_t* hash){
    if(!hash){
        printf("Error al intentar destruir el hash");
        return;
    }
    

    int i = 0;
    while(i < (hash->capacidad)){
        destruir_lista(hash->tabla[i]->nodo_inicio, hash->destructor);
        free(hash->tabla[i]);
        i++;
    }
    free(hash->tabla);
    free(hash);
}


size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
    if( !hash || !funcion || !aux ) return NULO;
    
    size_t iteraciones = 0;
    bool sigo_iterando = true;

    lista_iterador_t* iterador = NULL;
    size_t i = 0;
    while(i < (hash->capacidad)){
        if(hash->tabla[i]->nodo_inicio){
            for(iterador = lista_iterador_crear(hash->tabla[i]); (lista_iterador_tiene_siguiente(iterador) && sigo_iterando); lista_iterador_avanzar(iterador)){
                if(funcion(hash, iterador->corriente->clave, aux)){
                    sigo_iterando = false;
                }
                iteraciones++;
            }
            lista_iterador_destruir(iterador);
        }
        i++;
    }


    return iteraciones;
}