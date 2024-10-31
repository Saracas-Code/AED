//
// Created by Sarac on 21/10/2023.
//

#include "vacunodromo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Menú de la base de datos, con sus opciones
void menu_base_datos(TABB *A, char *fichero){

    char respuesta;

    do{
        printf("\n\n- Base de datos PacVac -\n\n");
        printf("A) Añadir un paciente\n");
        printf("L) Listar pacientes\n");
        printf("E) Eliminar un paciente\n");
        printf("S) Salir\n\n");
        printf("Seleccione una opción: ");
        scanf(" %c", &respuesta);

        switch(respuesta) {
            case 'A': case 'a':
                anhadirPaciente(A);
                cargar_archivo_salida(*A, fichero);
                break;
            case 'L': case 'l':
                listadoPacientes(*A);
                break;
            case 'E': case 'e':
                eliminarPaciente(A);
                cargar_archivo_salida(*A, fichero);
                break;
            case 'S': case 's':
                break;
            default:
                printf("Opción incorrecta");
                break;
        }

    }while(respuesta != 's' && respuesta!= 'S');
}

// Menú del vacunódromo, con sus opciones
void menu_vacunodromo(TABB *A, VACUNODROMO* vac, char *fichero){

    char respuesta;

    do{
        printf("\n\n- Vacunódromo PacVac -\n\n");
        printf("C) Crear cola de vacunación\n");
        printf("V) Vacunar\n");
        printf("S) Salir\n\n");
        printf("Seleccione una opción: ");
        scanf(" %c", &respuesta);

        switch(respuesta) {
            case 'C': case 'c':
                crear_cola_vacunacion(*A, vac);
                break;
            case 'V': case 'v':
                vacunar(vac, A);
                cargar_archivo_salida(*A, fichero);
                break;
            case 'S': case 's':
                break;
            default:
                printf("Opción incorrecta");
                break;
        }

    }while(respuesta != 's' && respuesta!= 'S');
}

// Programa principal
int main(int argc, char* argv[]){

    // Nombre del fichero de salida de los datos
    char nombreFichero[20];

    // Árbol de pacientes
    TABB arbolPacientes;
    crearAbb(&arbolPacientes);

    // Vacunódromo
    VACUNODROMO vacunodromo;
    crearCola(&vacunodromo.colaPacientes);

    // Si metemos un fichero de entrada, ya nos quedamos con ese nombre, si no, debemos ponerle un nombre al fichero de salida
    if (argc == 3){
        strcpy(nombreFichero, argv[2]);
        cargar_archivo_entrada(&arbolPacientes, argc, argv);
    }
    else{
        printf("Introduce un nombre para el fichero de salida: ");
        scanf(" %[^\n\r]", nombreFichero);
    }

    char respuesta;

    do{
        printf("\n---------------- PACVAC ----------------\n\n");
        printf("¡Bienvenida al programa de gestión PacVac!\n");
        printf("A) Gestión de la base de datos\n");
        printf("B) Gestión del vacunódromo\n");
        printf("S) Salir\n\n");
        printf("Seleccione una opción: ");
        scanf(" %c", &respuesta);

        switch(respuesta) {
            case 'A': case 'a':
                menu_base_datos(&arbolPacientes, nombreFichero);
                break;
            case 'B': case 'b':
                menu_vacunodromo(&arbolPacientes, &vacunodromo, nombreFichero);
                break;
            case 'S': case 's':
                printf("Saliendo del programa ...\n");
                destruirAbb(&arbolPacientes);
                destruirCola(&vacunodromo.colaPacientes);
                break;
            default:
                printf("Opción incorrecta");
                break;
        }

    }while(respuesta != 's' && respuesta!= 'S');

    return EXIT_SUCCESS;
}
