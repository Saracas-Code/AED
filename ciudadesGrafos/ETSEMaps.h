#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "grafo.h"

// FUNCIONES DEL PROGRAMA DE PRUEBA DE GRAFOS
// Opción a del menú, introducir un vertice en el grafo
void introducir_vertice(grafo *G);

// Opción b del menú, eliminar un vértice del grafo
void eliminar_vertice(grafo *G);

// Opción c del menú, crear una relación entre dos vértices
void nuevo_arco(grafo *G);

// Opción d del menú, eliminar una relación entre dos vértices
void eliminar_arco(grafo *G);

// Opción i del menú, imprimir el grafo
void imprimir_grafo(grafo G);

// Procesa el archivo de entrada y guarda la información en el grafo
void cargar_archivo_entrada(grafo *G, int nparam, char **args);

// Función para guardar los posibles cambios del fichero
void actualizar_fichero(grafo G, int argc, char **argv);

// Función del algoritmo de floyd warshall. Opciones e, f y g del menú
void floyd_warshall(grafo G, char tipo);

// Función del algoritmo de Prim. Opción h del menú
void prim(grafo G);

#endif	/* FUNCIONES_H */

