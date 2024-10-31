#include "tabla_hash_encadenamiento.h"

/* TABLA HASH CON ENCADENAMIENTO */

/******* FUNCIONES HASH *******/
int FuncionHash(char *cad, unsigned int tipoFH, unsigned int K) {
    int suma = 0, posicion = 0;
    switch (tipoFH) {
        case 1: //Método de la división: se suman los ASCII de los char y se calcula el resto módulo N
            for (int i = strlen(cad) - 1; i >= 0; i--)
                suma += cad[i];
            posicion = suma % N;
            break;
        case 2: case 3: //Suma ponderada de los valores ASCII
            for (int i = strlen(cad) - 1; i >= 0; i--)
                suma = (suma * K + cad[i]) % N;
            posicion = suma;
            break;
    }
    return posicion;
}

int InsertarHash(TablaHash *t, TIPOELEMENTO elemento, unsigned int tipoFH, unsigned int K) {

    int pos = FuncionHash(elemento.correo, tipoFH, K);

    int hayColision = 0;
    if (!esListaVacia((*t)[pos])) hayColision = 1;

    insertarElementoLista(&(*t)[pos], primeroLista((*t)[pos]), elemento);

    return hayColision;
}

int BuscarHash(TablaHash t, char *clavebuscar, TIPOELEMENTO *e, unsigned int tipoFH, unsigned int K, int *nPasosExtraB) {
    TPOSICION p;
    unsigned int encontrado = 0;
    TIPOELEMENTO ele;

    int pos = FuncionHash(clavebuscar, tipoFH, K);

    p = primeroLista(t[pos]);
    while (p != finLista(t[pos]) && !encontrado) {
        recuperarElementoLista(t[pos], p, &ele);
        if (strcmp(ele.correo, clavebuscar) == 0) {
            encontrado = 1;
            *e = ele;
        } else {
            p = siguienteLista(t[pos], p);
            (*nPasosExtraB)++;
        }
    }
    return encontrado;
}

/* DEVUELVE 1 SI EL ELEMENTO e ESTA EN LA TABLA Y 0 SI NO LO ESTA */
int EsMiembroHash(TablaHash t, char *clavebuscar, unsigned int tipoFH, unsigned int K, int *nPasosExtraB) {

    TPOSICION p;
    int encontrado = 0;
    TIPOELEMENTO elemento;
    int pos = FuncionHash(clavebuscar, tipoFH, K);
    p = primeroLista(t[pos]);

    while (p != finLista(t[pos]) && !encontrado) {
        recuperarElementoLista(t[pos], p, &elemento);
        if (strcmp(clavebuscar, elemento.correo) == 0)
            encontrado = 1;
        else {
            p = siguienteLista(t[pos], p);
            (*nPasosExtraB)++;
        }
    }

    return encontrado;
}

void BorrarHash(TablaHash *t, char *claveborrar, unsigned int tipoFH, unsigned int K, int *nPasosExtraE) {

    TPOSICION p;
    TIPOELEMENTO elemento;
    int pos = FuncionHash(claveborrar, tipoFH, K);

    p = primeroLista((*t)[pos]);
    recuperarElementoLista((*t)[pos], p, &elemento);

    while (p != finLista((*t)[pos]) && strcmp(claveborrar, elemento.correo)) {
        p = siguienteLista((*t)[pos], p);
        recuperarElementoLista((*t)[pos], p, &elemento);
        (*nPasosExtraE)++;  // Incrementar nPasosExtraE por cada paso adicional.
    }
    if (p != finLista((*t)[pos]))
        suprimirElementoLista(&(*t)[pos], p);
}


/* CREAR TABLA VACIA */
void InicializarTablaHash(TablaHash t) {
    for (int i = 0; i < N; i++)
        crearLista(&t[i]);
}

/* DESTRUIR TABLA */
void DestruirTablaHash(TablaHash t) {
    for (int i = 0; i < N; i++)
        destruirLista(&t[i]);
}
