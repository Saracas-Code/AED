//
// Created by Sara Castro on 19/10/2023.
//

#ifndef ENTREGASESION3_VACUNODROMO_H
#define ENTREGASESION3_VACUNODROMO_H


#include "abb.h"
#include "cola.h"

typedef struct{
    char nombreVacuna[MAXVACUNA];
    TCOLA colaPacientes;
    int nPacientes;
} VACUNODROMO;

// Función para añadir un paciente al árbol de la base de datos
void anhadirPaciente(TABB *arbolPacientes);

// Función para imprimir los pacientes de la base de datos
void listadoPacientes(TABB arbolPacientes);

// Función para eliminar un paciente de la base de datos en función del nombre que se introduzca
void eliminarPaciente(TABB *arbolPacientes);

// Función para coger pacientes de un archivo de entrada y meterlos en la base de datos
void cargar_archivo_entrada(TABB *A, int nparam, char **args);

// Función para imprimir en el archivo la lista de pacientes
void cargar_archivo_salida(TABB A, char *nombreFichero);

// Función para crear un vacunódromo con una cola de pacientes
void crear_cola_vacunacion(TABB A, VACUNODROMO *V);

// Función para vacunar al primer paciente de la cola de vacunación del vacunódromo
void vacunar(VACUNODROMO *V, TABB *arbolPacientes);

#endif //ENTREGASESION3_VACUNODROMO_H
