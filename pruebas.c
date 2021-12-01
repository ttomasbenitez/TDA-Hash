#include "pa2mm.h"
#include "src/hash.h"
#include "src/struct_hash.h"


size_t funcion_hash_aux(const char* clave, size_t capacidad){
    size_t posicion = 0;

    while(*clave){
        posicion += *(clave++);
    }
    return (posicion%capacidad);
}


void Puedo_Insertar_Correctamente_Elementos(){
    size_t capacidad = 2;
    hash_t* hash = hash_crear(NULL, capacidad);

    capacidad = (hash->capacidad);


    char* Auto_1 = "Volkswagen";
    char* Patente_1 = "AC222CA";
    char* Auto_2 = "Fiat";
    char* Patente_2 = "SSQ777";
    char* Auto_3 = "Renault";
    char* Patente_3 = "SSS777";
    char* Auto_4 = "Nissan";
    char* Patente_4 = "AQ000QW";
    char* Auto_5 = NULL;
    char* Patente_5 = "ABB532";
    char* Auto_6 = "Toyota";
    char* Patente_6 = NULL;
    


    
    size_t pos_insertado = funcion_hash_aux(Patente_1, capacidad);
    hash_insertar(hash, Patente_1, Auto_1);
    pa2m_afirmar((hash->tabla[pos_insertado]->nodo_inicio->elemento) == Auto_1, "Se insertó correctamente un elemento");

    pos_insertado = funcion_hash_aux(Patente_2, capacidad);
    hash_insertar(hash, Patente_2, Auto_2);
    pa2m_afirmar((hash->tabla[pos_insertado]->nodo_inicio->elemento) == Auto_2, "Se insertaron correctamente dos elementos");


    pos_insertado = funcion_hash_aux(Patente_3, capacidad);
    hash_insertar(hash, Patente_3, Auto_3);
    pa2m_afirmar((hash->tabla[pos_insertado]->nodo_fin->elemento) == Auto_3, "Se enlistaron dos elementos correctamente");


    hash_insertar(hash, Patente_4, Auto_4);
    capacidad = (hash->capacidad);
    pos_insertado = funcion_hash_aux(Patente_4, capacidad);
    pa2m_afirmar((hash->tabla[pos_insertado]->nodo_inicio->elemento)== Auto_4, "Agrego un elemento y tengo que rehashear, se agrega correctamente el elemento");


    pos_insertado = funcion_hash_aux(Patente_2, capacidad);
    size_t pos_insertado_2 = funcion_hash_aux(Patente_1, capacidad); 
    size_t pos_insertado_3 = funcion_hash_aux(Patente_3, capacidad); 

    pa2m_afirmar( ((hash->tabla[pos_insertado]->nodo_inicio->elemento) == Auto_2) && 
    ((hash->tabla[pos_insertado_2]->nodo_inicio->elemento) == Auto_1) &&
    ((hash->tabla[pos_insertado_3]->nodo_fin->elemento) == Auto_3) , "Luego del rehash, los elementos que ya estaban, siguen estando");
    

    pos_insertado = funcion_hash_aux(Patente_5, capacidad);
    hash_insertar(hash, Patente_5, Auto_5);
    pa2m_afirmar((hash->tabla[pos_insertado]->nodo_inicio->elemento) == NULL, "Puedo insertar un elemento NULL");
    
    
    pa2m_afirmar((hash_insertar(hash, Patente_6, Auto_6) == -1) , "No puedo insertar un elemento con clave NULL");
    

    hash_destruir(hash);
}


void Puedo_Borrar_Elementos(){
    size_t capacidad = 2;
    hash_t* hash = hash_crear(NULL, capacidad);

    char* Auto_1 = "Volkswagen";
    char* Patente_1 = "AC222CA";
    char* Auto_2 = "Fiat";
    char* Patente_2 = "SSQ777";
    char* Auto_3 = "Renault";
    char* Patente_3 = "SSS777";
    char* Auto_4 = "Nissan";
    char* Patente_4 = "AQ000QW";
    char* Auto_5 = NULL;
    char* Patente_5 = "ABB532";
    char* Auto_6 = "Toyota";
    char* Patente_6 = "SA111SA";
        
    hash_insertar(hash, Patente_1, Auto_1);
    hash_insertar(hash, Patente_2, Auto_2); 
    hash_insertar(hash, Patente_3, Auto_3);
    hash_insertar(hash, Patente_4, Auto_4);
    hash_insertar(hash, Patente_5, Auto_5);
    hash_insertar(hash, Patente_6, Auto_6);


    ///******   QUITAR  **********
    
    capacidad = (hash->capacidad);
    size_t pos_insertado = funcion_hash_aux(Patente_4, capacidad);
    int quitado = hash_quitar(hash, Patente_4);
    pa2m_afirmar(((hash->tabla[pos_insertado]->nodo_inicio) == NULL) && (quitado == 0), "Elimino correctamente un elemento");

    quitado = hash_quitar(hash, Patente_4);
    pa2m_afirmar(quitado == -1 ,"No puedo quitar un elemento que no está en el hash");
    pa2m_afirmar((hash_cantidad(hash) == 5),"No puedo quitar un elemento que no está en el hash, por lo tanto, la cantidad de elementos en el hash se mantiene igual");

    quitado = hash_quitar(hash, NULL);
    pa2m_afirmar(quitado == -1 ,"No puedo quitar algo con clave inválida (NULL)");

    hash_quitar(hash, Patente_2);
    hash_quitar(hash, Patente_6);

    pa2m_afirmar((hash_cantidad(hash) == 3), "Quito tres elementos, el hash tiene la cantidad correcta");
    

    hash_quitar(hash, Patente_5);
    hash_quitar(hash, Patente_1);
    hash_quitar(hash, Patente_3);

    pa2m_afirmar((hash_cantidad(hash) == 0), "Quito todos los elementos, el hash está vacío");
    

    pa2m_afirmar( ((hash_quitar(hash, Patente_1) == -1) && (hash_quitar(hash, Patente_2) == -1) && (hash_quitar(hash, Patente_3) == -1) &&
    (hash_quitar(hash, Patente_4) == -1) && (hash_quitar(hash, Patente_5) == -1) && (hash_quitar(hash, Patente_6) == -1) &&
    (hash_quitar(hash, NULL) == -1) && (hash_quitar(hash, "ASS111") == -1) ), "No puedo quitar elementos de un Hash vacío");



    hash_destruir(hash);
    
}


void Puedo_Buscar_Elementos(){
    size_t capacidad = 2;
    hash_t* hash = hash_crear(NULL, capacidad);

    char* Auto_1 = "Volkswagen";
    char* Patente_1 = "AC222CA";
    char* Auto_2 = "Fiat";
    char* Patente_2 = "SSQ777";
    char* Auto_3 = "Renault";
    char* Patente_3 = "SSS777";
    char* Auto_4 = "Nissan";
    char* Patente_4 = "AQ000QW";
    char* Auto_5 = NULL;
    char* Patente_5 = "ABB532";
    char* Auto_6 = "Toyota";
    char* Patente_6 = "SA111SA";

    char* Patente_Inexistente = "AVV555";
    
    hash_insertar(hash, Patente_1, Auto_1);
    hash_insertar(hash, Patente_2, Auto_2); 
    hash_insertar(hash, Patente_3, Auto_3);
    hash_insertar(hash, Patente_4, Auto_4);
    hash_insertar(hash, Patente_5, Auto_5);
    hash_insertar(hash, Patente_6, Auto_6);

    //******* BÚSQUEDA *******

    int contiene = hash_contiene(hash, NULL);
    pa2m_afirmar(contiene == 0, "El Hash no contiene elementos con clave inválida (NULL)");

    contiene = hash_contiene(hash, Patente_Inexistente);
    pa2m_afirmar(contiene == 0, "El Hash no contiene al elemento buscado");

    contiene = hash_contiene(hash, Patente_4);
    pa2m_afirmar(contiene != 0, "El Hash contiene al elemento buscado");


    void* elemento = hash_obtener(hash, NULL);
    pa2m_afirmar(elemento == NULL, "Quiero obtener elemento con clave inválida, devuelve NULL");

    elemento = hash_obtener(hash, Patente_Inexistente);
    pa2m_afirmar(elemento == NULL, "Quiero obtener elemento que no está en el Hash, devuelve NULL");

    elemento = hash_obtener(hash, Patente_3);
    pa2m_afirmar(elemento == Auto_3, "Obtengo correctamente el elemento buscado");

    hash_quitar(hash, Patente_3);
    elemento = hash_obtener(hash, Patente_3);
    pa2m_afirmar(elemento == NULL, "Quito el elemento, ya no lo puedo obtener");


    hash_quitar(hash, Patente_1);
    hash_quitar(hash, Patente_2);
    hash_quitar(hash, Patente_4);
    hash_quitar(hash, Patente_5);
    hash_quitar(hash, Patente_6);


    pa2m_afirmar( ((hash_contiene(hash, Patente_1) == 0) && (hash_contiene(hash, Patente_2) == 0) && (hash_contiene(hash, Patente_3) == 0) &&
    (hash_contiene(hash, Patente_4) == 0) && (hash_contiene(hash, Patente_5) == 0) && (hash_contiene(hash, Patente_6) == 0) &&
    (hash_contiene(hash, NULL) == 0) && (hash_contiene(hash, Patente_Inexistente) == 0) ), "Un Hash vacío no contiene elementos");



    pa2m_afirmar( ((hash_obtener(hash, Patente_1) == 0) && (hash_obtener(hash, Patente_2) == 0) && (hash_obtener(hash, Patente_3) == 0) &&
    (hash_obtener(hash, Patente_4) == 0) && (hash_obtener(hash, Patente_5) == 0) && (hash_obtener(hash, Patente_6) == 0) &&
    (hash_obtener(hash, NULL) == 0) && (hash_obtener(hash, Patente_Inexistente) == 0) ), "No puedo obtener elementos de un Hash vacío");


    hash_destruir(hash);
}


bool seguir_iterando(hash_t* hash, const char* clave, void* aux){
    if(strcmp(clave, aux) == 0){
        return true;
    }
    return false;
}


void Puedo_Usar_el_Iterador_Interno(){
    size_t capacidad = 2;
    hash_t* hash = hash_crear(NULL, capacidad);

    char* Auto_1 = "Volkswagen";
    char* Patente_1 = "AC222CA";
    char* Auto_2 = "Fiat";
    char* Patente_2 = "SSQ777";
    char* Auto_3 = "Renault";
    char* Patente_3 = "SSS777";
    char* Auto_4 = "Nissan";
    char* Patente_4 = "AQ000QW";
    char* Auto_5 = NULL;
    char* Patente_5 = "ABB532";
    char* Auto_6 = "Toyota";
    char* Patente_6 = "SA111SA";

    char* Patente_Inexistente = "AVV555";
    
    hash_insertar(hash, Patente_1, Auto_1);
    hash_insertar(hash, Patente_2, Auto_2); 
    hash_insertar(hash, Patente_3, Auto_3);
    hash_insertar(hash, Patente_4, Auto_4);
    hash_insertar(hash, Patente_5, Auto_5);
    hash_insertar(hash, Patente_6, Auto_6); 


    size_t iteraciones = hash_con_cada_clave(hash, seguir_iterando, Patente_Inexistente);
    pa2m_afirmar(iteraciones == (hash->cantidad_elementos) , "Itero hasta encontrar una clave inexistente, itera tantas veces como cantidad de elementos haya");

    iteraciones = hash_con_cada_clave(hash, seguir_iterando, Patente_1);
    pa2m_afirmar(iteraciones == 1 , "Itero hasta encontrar el elemento en la posición 0 de la tabla, itera una vez");

    iteraciones = hash_con_cada_clave(hash, NULL, Patente_1);
    pa2m_afirmar(iteraciones == 0 , "La función pasada es inválida, no itera");

    iteraciones = hash_con_cada_clave(hash, seguir_iterando, NULL);
    pa2m_afirmar(iteraciones == 0 , "Busco elemento con clave inválida, no es válido y por ello no itera");


    hash_destruir(hash);

}


void Puedo_Usar_un_Hash_Nulo(){
    hash_t* hash = NULL;

    char* Auto_1 = "Volkswagen";
    char* Patente_1 = "AC222CA";
    char* Auto_2 = "Fiat";
    char* Patente_2 = "SSQ777";
    char* Auto_3 = "Renault";
    char* Patente_3 = "SSS777";

    pa2m_afirmar( ((hash_insertar(hash, Patente_1, Auto_1) == -1) && (hash_insertar(hash, Patente_2, Auto_2) == -1) && 
    (hash_insertar(hash, Patente_3, Auto_3) == -1) ), "No puedo insertar elementos en un Hash NULL");


    pa2m_afirmar( ((hash_contiene(hash, Patente_1) == 0) && (hash_contiene(hash, Patente_2) == 0) && (hash_contiene(hash, Patente_3) == 0) &&
    (hash_contiene(hash, NULL) == 0) ), "Un Hash NULL no contiene elementos");

    
    pa2m_afirmar( ((hash_obtener(hash, Patente_1) == 0) && (hash_obtener(hash, Patente_2) == 0) && (hash_obtener(hash, Patente_3) == 0) &&
    (hash_obtener(hash, NULL) == 0) ), "No puedo obtener elementos de un Hash NULL");

    
    pa2m_afirmar( ((hash_quitar(hash, Patente_1) == -1) && (hash_quitar(hash, Patente_2) == -1) && (hash_quitar(hash, Patente_3) == -1) &&
    (hash_quitar(hash, NULL) == -1) ), "No puedo quitar elementos de un Hash NULL");


    pa2m_afirmar((hash_cantidad(hash) == 0), "Un Hash NULL tiene cantidad 0");
}



int main(){
    pa2m_nuevo_grupo("Pruebas de Inserción");
    Puedo_Insertar_Correctamente_Elementos();
    
    pa2m_nuevo_grupo("Pruebas de Eliminación");
    Puedo_Borrar_Elementos();

    pa2m_nuevo_grupo("Pruebas de Búsqueda");
    Puedo_Buscar_Elementos();
    
    pa2m_nuevo_grupo("Pruebas de Iterador Interno");
    Puedo_Usar_el_Iterador_Interno();


    pa2m_nuevo_grupo("Pruebas con Hash NULL");
    Puedo_Usar_un_Hash_Nulo();

    return pa2m_mostrar_reporte();
}