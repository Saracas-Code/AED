#include <stdio.h>
#include <stdlib.h>
#include "ETSEMaps.h"

/*
 * Programa que muestra el uso del TAD grafo de números enteros
 */

int main(int argc, char** argv) {

    // Grafo de nombres de ciudades
    grafo G; //grafo
    char opcion;

    // Creo el grafo
    crear_grafo(&G);

    // Procesamos el archivo de entrada
    cargar_archivo_entrada(&G, argc, argv);

    do {
        printf("\n\n------------ ETSEMaps --------------\n\n");
        printf("¡Bienvenidx al programa ETSEMaps!\n\n");
        printf("MODIFICAR EL MAPA\n\n");
        printf("a. Añadir nueva ciudad\n");
        printf("b. Borrar ciudad\n");
        printf("c. Construir vía entre ciudades\n");
        printf("d. Descartar vía entre ciudades\n\n");
        printf("ENCONTRAR RUTAS\n\n");
        printf("e. Ruta más ECONÓMICA\n");
        printf("f. Ruta más CORTA\n");
        printf("g. Ruta más RÁPIDA\n\n");
        printf("h. Ruta más RÁPIDA que conecte TODAS LAS CIUDADES\n\n");
        printf("VISUALIZACIÓN\n\n");
        printf("i. Imprimir mapa\n\n\n\n");
        printf("Pulse S/s si quiere salir del programa\n\n");

        printf("Opción: ");
        scanf(" %c", &opcion); 

        switch (opcion) {
            case 'a':case'A':
                introducir_vertice(&G);
                break;
            case 'b':case 'B':
                eliminar_vertice(&G);
                break;
            case 'c': case 'C':
                nuevo_arco(&G);
                break;
            case 'd': case 'D':
                eliminar_arco(&G);
                break;
            case 'e': case 'E': case 'f': case 'F': case 'g': case 'G':
                floyd_warshall(G, opcion);
                break;
            case 'h': case 'H':
                prim(G);
                break;
            case 'i': case 'I':
                imprimir_grafo(G);
                break;
            case 's': case 'S':
                printf("Saliendo del programa ...");
                break;
            default:
                printf("Opción equivocada\n");
        }
    } while (opcion != 's' && opcion != 'S');

    //Actualizar fichero antes de salir del programa
    actualizar_fichero(G, argc, argv);

    //Al salir, liberamos la memoria del TAD, lo destruimos
    borrar_grafo(&G);

    return (EXIT_SUCCESS);
}

