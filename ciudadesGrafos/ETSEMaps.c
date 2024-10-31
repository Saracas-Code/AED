#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "grafo.h"

#define COSTE_CARRETERA (float) 0.01
#define COSTE_AUTOPISTA (float) 0.07
#define VELOCIDAD_CARRETERA 70
#define VELOCIDAD_AUTOPISTA 120

typedef struct {
    char tipo;
    int pos;
} predecesor;

//FUNCIONES DEL PROGRAMA DE PRUEBA DE GRAFOS

//Opción a del menú, introducir un vertice en el grafo
void introducir_vertice(grafo *G) {

    tipovertice v1;
    printf("\nIntroduce el nombre de la ciudad que deseas añadir (vértice): ");
    scanf(" %[^\r\n]", v1.nombrePoblacion);

    if (existe_vertice(*G, v1))
        printf("Esa ciudad ya esta en el mapa\n");
    else
        insertar_vertice(G, v1);

    printf("VÉRTICE CREADO\n\n");
}

//Opción b del menú, eliminar un vértice del grafo
void eliminar_vertice(grafo *G) {

    tipovertice v1;
    printf("Introduce el nombre de la ciudad que deseas eliminar del mapa (vértice):");
    scanf(" %[^\r\n]", v1.nombrePoblacion);

    if (existe_vertice(*G, v1))
        borrar_vertice(G, v1);
    else
        printf("La ciudad %s no existe en el mapa\n", v1.nombrePoblacion);

    printf("VÉRTICE ELIMINADO\n\n");
}

//Opción c del menú, crear una relación entre dos vértices
void nuevo_arco(grafo *G) {

    tipovertice v1, v2;
    float valor;
    char tipo;

    do {
        printf("¿Qué tipo de vía quieres crear?\nC) Carretera\nA) Autopista\nSelecciona una opción: ");
        scanf(" %c", &tipo);
    } while (tipo != 'C' && tipo != 'c' && tipo != 'A' && tipo != 'a');

    //Insertamos una nueva relación pidiendo los datos al usuario controlando que existan los vértices
    printf("\nNueva relación Ciudad1 --> Ciudad2\n\n");

    //Vértice origen del arco
    printf("Introduce la ciudad de origen: ");
    scanf(" %[^\r\n]", v1.nombrePoblacion);

    if (!existe_vertice(*G, v1)) {
        printf("La ciudad %s no existe en el mapa\n", v1.nombrePoblacion);
        return;
    }

    //Vértice destino del arco
    printf("Introduce la ciudad de destino: ");
    scanf(" %[^\r\n]", v2.nombrePoblacion);

    if (!existe_vertice(*G, v2)) {
        printf("La ciudad %s no existe en el mapa\n", v2.nombrePoblacion);
        return;
    }

    //Distancia del arco
    printf("Introduce la distancia entre las dos ciudades: ");
    scanf(" %f", &valor);

    //Creación del arco
    if (distancia(*G, posicion(*G, v1), posicion(*G, v2), tipo) == 0)
        crear_arco(G, posicion(*G, v1), posicion(*G, v2), valor, tipo);

    printf("\nARCO CREADO\n\n");
}

//Opción d del menú, eliminar una relación entre dos vértices
void eliminar_arco(grafo *G) {

    tipovertice v1, v2;
    char tipo;

    do {
        printf("¿Qué tipo de vía quieres eliminar? (Asegúrate de que exista previamente)\nC) Carretera\nA) Autopista\nSelecciona una opción: ");
        scanf(" %c", &tipo);
    } while (tipo != 'C' && tipo != 'c' && tipo != 'A' && tipo != 'a');

    //Eliminamos una relación pidiendo los datos al usuario controlando que existan los vértices
    printf("\nEliminar relación Ciudad1-->Ciudad2\n\n");
    //Vértice origen del arco
    printf("Introduce la ciudad de origen: ");
    scanf(" %[^\r\n]", v1.nombrePoblacion);

    if (!existe_vertice(*G, v1)) {
        printf("La ciudad %s no existe en el mapa\n", v1.nombrePoblacion);
        return;
    }

    //Vértice destino del arco
    printf("Introduce la ciudad de destino: ");
    scanf(" %[^\r\n]", v2.nombrePoblacion);

    if (!existe_vertice(*G, v2)) {
        printf("La ciudad %s no existe en el mapa\n", v2.nombrePoblacion);
        return;
    }

    //Eliminación del arco
    if (distancia(*G, posicion(*G, v1), posicion(*G, v2), tipo) == 0)
        borrar_arco(G, posicion(*G, v1), posicion(*G, v2), tipo);

    printf("ARCO ELIMINADO\n\n");
}

//Opción i del menú, imprimir el grafo
//Función que imprime el grafo utilizando num_vertices para saber cuántos vértices tiene
//y array_vertices para recuperar el vector de vértices y recorrerlo
void imprimir_grafo(grafo G) {

    if (G == NULL) {
        printf("Grafo no inicializado.\n");
        return;
    }

    int numVertices = num_vertices(G);
    tipovertice* vertices = array_vertices(G);

    // Imprimir información sobre los vértices
    printf("\n-- NÚMERO DE VÉRTICES: %d --\n", numVertices);
    for (int i = 0; i < numVertices; i++) {
        printf("Vértice %d: %s\n", i+1, vertices[i].nombrePoblacion);
    }

    // Imprimir la matriz de adyacencia de carreteras
    printf("\n-- CONEXIONES ENTRE CIUDADES --\n");
    for (int i = 0; i < numVertices; i++) {
        //Imprimo el vértice
        printf("Vértice(%d): %s\n", i+1, vertices[i].nombrePoblacion);
        //Chequeo sus arcos
        for (int j = 0; j < numVertices; j++){
            if (distancia(G, i, j, 'C') > 0)
                printf("\t%s --> %s (%.3f kms)\n", vertices[i].nombrePoblacion, vertices[j].nombrePoblacion, distancia(G, i, j, 'C'));
            if (distancia(G, i, j, 'A') > 0)
                printf("\t%s ==> %s (%.3f kms)\n", vertices[i].nombrePoblacion, vertices[j].nombrePoblacion, distancia(G, i, j, 'A'));
        }
        printf("\n");
    }


}

//Función a la que se llama al inicio del programa main, para procesar el mapa
//inicial de ciudades y conexiones
void cargar_archivo_entrada(grafo *G, int argc, char **argv){

    // Comprobación de parámetros
    if(argc >= 3 && (strcmp("-f", argv[1])==0)){

        // Abrimos el fichero en modo lectura
        FILE *fp = fopen(argv[2], "r");

        if (fp == NULL) printf("\nERROR AL ABRIR EL ARCHIVO\n\n");
        else {

            fseek(fp, 0, SEEK_SET); // Nos posicionamos al inicio del fichero

            // Declaración de variables
            tipovertice v1, v2;
            char *patron = "%c, %[^,], %[^,], %f", tipoVia;
            float valorDistancia;

            while(!feof(fp)) {

                // Escaneamos los datos en base al patrón que hay en el fichero
                fscanf(fp, patron, &tipoVia, v1.nombrePoblacion, v2.nombrePoblacion, &valorDistancia);

                // Insertamos el vértice si no está en el grafo
                if(!existe_vertice(*G, v1)) insertar_vertice(G, v1);
                if(!existe_vertice(*G, v2)) insertar_vertice(G, v2);

                // Creación del arco
                if (distancia(*G, posicion(*G, v1), posicion(*G, v2), tipoVia) == 0)
                    crear_arco(G, posicion(*G, v1), posicion(*G, v2), valorDistancia, tipoVia);
            }
        }
        fclose(fp);
    }
    else printf("\nProcesar un fichero de conexiones entre ciudades.\nFORMATO: $>./ejecutable -f archivo.txt\n");
}

//Función para guardar los posibles cambios del fichero
void actualizar_fichero(grafo G, int argc, char **argv) {

    if (G == NULL) {
        printf("Grafo no inicializado.\n");
        return;
    }

    if(argc >= 3 && (strcmp("-f", argv[1])==0)) {

        // Abrimos el fichero en modo escritura
        FILE *fp = fopen(argv[2], "w");

        if (fp == NULL) printf("\nERROR AL ABRIR EL ARCHIVO\n\n");
        else {

            int numVertices = num_vertices(G);
            tipovertice* vertices = array_vertices(G);

            for (int i = 0; i < numVertices; i++) {
                for (int j = i+1; j < numVertices; j++) {
                    if (distancia(G, i, j, 'C') > 0)
                        fprintf(fp, "C, %s, %s, %f\n", vertices[i].nombrePoblacion, vertices[j].nombrePoblacion, distancia(G, i, j, 'C'));
                    if (distancia(G, i, j, 'A') > 0)
                        fprintf(fp, "A, %s, %s, %f\n", vertices[i].nombrePoblacion, vertices[j].nombrePoblacion, distancia(G, i, j, 'A'));
                }
            }
        }
    }
}

// Función buscar camino
void buscar_camino(int nVertices, float matrizD[nVertices][nVertices], predecesor matrizP[nVertices][nVertices]) {
    for (int k=0; k<nVertices; k++) {
        for (int i=0; i<nVertices; i++) {
            for (int j=0; j<nVertices; j++) {
                if (matrizD[i][j] > matrizD[i][k] + matrizD[k][j]){
                    matrizD[i][j] = matrizD[i][k] + matrizD[k][j];
                    matrizP[i][j] = matrizP[k][j];
                }
            }
        }
    }
}

// Función imprimir camino
void imprimir_camino(grafo G, int nVertices, predecesor matrizP[nVertices][nVertices], int origen, int destino) {

    tipovertice* vertices = array_vertices(G);

    if (origen != destino)
        imprimir_camino(G, nVertices, matrizP, origen, matrizP[origen][destino].pos);

    switch (matrizP[origen][destino].tipo) { // Imprimimos
        case 'C':
            printf("-->");
            break;
        case 'A':
            printf("==>");
    }
    printf(" %s ", vertices[destino].nombrePoblacion);

}

// Inicialización matriz D y P
void inicializar_matrices(grafo G, int numV, float matrizD[numV][numV], predecesor matrizP[numV][numV], float factorA, float factorC) {

    for (int i = 0; i < numV; i++) {
        for (int j = 0; j < numV; j++) {
            if (i == j) { // No hay caminos Santiago - Santiago, Pontevedra - Pontevedra
                matrizD[i][j] = 0;
                matrizP[i][j].tipo = ' ';
                matrizP[i][j].pos = 0;
            }
            else if (distancia(G, i, j, 'A') || distancia(G, i, j, 'C')) { // Comprobamos si existe vía, ya sea por autopista o por carretera
                if (!distancia(G, i, j, 'A') || distancia(G, i, j, 'A')*factorA > distancia(G, i, j, 'C')*factorC) { // Si no existe autopista, o la distancia por carretera
                    matrizD[i][j] = distancia(G, i, j, 'C')*factorC;                                                 // es menor que la de autopista, nos quedamos con la de
                    matrizP[i][j].tipo = 'C';                                                                        // carretera
                    matrizP[i][j].pos = i;
                }
                else if (!distancia(G, i, j, 'C') || distancia(G, i, j, 'C')*factorC > distancia(G, i, j, 'A')*factorA) { // Si no existe carretera, o la distancia por autopista
                    matrizD[i][j] = distancia(G, i, j, 'A')*factorA;                                                      // es menor que la de carretera, nos quedamos con la de
                    matrizP[i][j].tipo = 'A';                                                                             // autopista
                    matrizP[i][j].pos = i;
                }
            }
            else { // Si no existe ninguna vía, la distancia será infinita ya que no habrá conexión directa entre esas ciudades
                matrizD[i][j] = INFINITY;
                matrizP[i][j].tipo = ' ';
                matrizP[i][j].pos = 0;
            }
        }
    }
}

// Función del algoritmo de floyd warshall. Opciones e, f y g del menú
void floyd_warshall(grafo G, char opcion) {

    int numV = num_vertices(G);
    tipovertice vOrigen, vDestino;
    float multC, multA;
    float D[numV][numV]; // D: Matriz de caminos mínimos del grafo
    predecesor P[numV][numV]; // P: Matriz de vértices predecesores

    // Pedimos las ciudades al usuario
    printf("Introduce la ciudad de origen: ");
    scanf(" %[^\r\n]", vOrigen.nombrePoblacion);
    if (!existe_vertice(G, vOrigen)) {
        printf("La ciudad %s no existe en el mapa\n", vOrigen.nombrePoblacion);
        return;
    }

    printf("Introduce la ciudad de destino: ");
    scanf(" %[^\r\n]", vDestino.nombrePoblacion);
    if (!existe_vertice(G, vDestino)) {
        printf("La ciudad %s no existe en el mapa\n", vDestino.nombrePoblacion);
        return;
    }
    
    switch (opcion) {
        case 'e': case 'E': // Minimizar el coste económico
            multC = COSTE_CARRETERA;
            multA = COSTE_AUTOPISTA;
            break;
            case 'f': case 'F': // Minimizar el coste en distancia (kms)
            multC = 1;
            multA = 1;
            break;
            case 'g': case 'G': // Minimizar el coste en tiempo (horas)
            multC = (float) 1/VELOCIDAD_CARRETERA;
            multA = (float) 1/VELOCIDAD_AUTOPISTA;
            break;
    }

    inicializar_matrices(G, numV, D, P, multA, multC); // Inicializamos las matrices D y P
    buscar_camino(numV, D, P); // Buscamos el camino

    // Imprimimos la ruta final
    switch (opcion) {
        case 'e':
            printf("\n\nEl coste económico total desde %s hasta %s es de %.2f €", vOrigen.nombrePoblacion, vDestino.nombrePoblacion, D[posicion(G, vOrigen)][posicion(G, vDestino)]);
            break;
        case 'f':
            printf("\n\nLa distancia total desde %s hasta %s es de %.2f kms", vOrigen.nombrePoblacion, vDestino.nombrePoblacion, D[posicion(G, vOrigen)][posicion(G, vDestino)]);
            break;
        case 'g':
            printf("\n\nEl tiempo total desde %s hasta %s es de %.2f horas", vOrigen.nombrePoblacion, vDestino.nombrePoblacion, D[posicion(G, vOrigen)][posicion(G, vDestino)]);
            break;
    }
    printf("\n\n\t\tRuta: ");
    imprimir_camino(G, numV, P, posicion(G, vOrigen), posicion(G, vDestino));
}

// Función del algoritmo de Prim. Opción h del menú
void prim(grafo G) {

    int numV = num_vertices(G);
    tipovertice* vertices = array_vertices(G);
    int Selected[numV];
    char tipoVia;
    int numArcos = 0;
    float tCarretera, tAutopista;
    double minimo, distanciaTotal = 0;

    for (int i=0; i<numV; i++)
        Selected[i] = 0;

    Selected[0] = 1;

    printf("\n");
    while (numArcos < numV - 1) {

        minimo = INFINITY;
        int vx = 0, vy = 0;

        for (int i = 0; i < numV; i++) {
            if (Selected[i] == 1) {
                for (int j = 0; j < numV; j++) {

                    // Guardamos el tiempo por carretera y el tiempo por autopista
                    tCarretera = distancia(G, i, j, 'C')/VELOCIDAD_CARRETERA;
                    tAutopista = distancia(G, i, j, 'A')/VELOCIDAD_AUTOPISTA;

                    // Nos quedamos con el tiempo mínimo, sea carretera o autopista
                    if (Selected[j] != 1 && (tAutopista || tCarretera)) {
                        if (!tAutopista || (tAutopista > tCarretera)) {
                            if (minimo > tCarretera) {
                                minimo = tCarretera;
                                vx = i;
                                vy = j;
                                tipoVia = 'C';
                            }
                        }
                        else if (!tCarretera || (tCarretera > tAutopista)) {
                            if (minimo > tAutopista) {
                                minimo = tAutopista;
                                vx = i;
                                vy = j;
                                tipoVia = 'A';
                            }
                        }
                    }
                }
            }
        }

        Selected[vy] = 1;
        numArcos++;

        // Imprimimos cada conexión entre ciudades, atendiendo a si las hemos conectado por autopista o por carretera
        printf("%-10s ", vertices[vx].nombrePoblacion);
        if(tipoVia == 'C') {
            printf("--> %10s : %10.2f horas\n", vertices[vy].nombrePoblacion, minimo);
            distanciaTotal += minimo;
        }

        else {
            printf("==> %10s : %10.2f horas\n", vertices[vy].nombrePoblacion, minimo);
            distanciaTotal += minimo;
        }
    }
    printf("\nTiempo del árbol de expansión de costo mínimo = %.2f horas\n", distanciaTotal);
}
