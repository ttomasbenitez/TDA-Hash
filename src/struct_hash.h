#include "hash.h"
#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct hash{
    lista_t** tabla;
    size_t capacidad;
    size_t cantidad_elementos;
    hash_destruir_dato_t destructor;
};
