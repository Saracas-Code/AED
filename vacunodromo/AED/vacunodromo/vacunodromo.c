//
// Created by Sara Castro on 19/10/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "vacunodromo.h"
#include "abb.h"
#include "cola.h"

#define MAXLINEA 100

///////////////////// FUNCIONES OCULTAS ////////////////////////
// Imprimir por pantalla. Cada función llama a la anterior
void _imprimirVacunas(TLISTA listaVac){
    TIPOELEMENTOLISTA vac;
    // Iteramos en la lista de vacunas
    for(TPOSICION i = primeroLista(listaVac); i < finLista(listaVac); i = siguienteLista(listaVac, i)){
        recuperarElementoLista(listaVac, i, &vac);
        printf("\tVacuna: %s. Dosis %d de %d\n", vac.nombreVacuna, vac.nDosis, vac.maxDosis);
    }
}
void _imprimirPaciente(TIPOELEMENTOABB paciente){
    printf("\nNombre: %s\n", paciente.ApN);
    _imprimirVacunas(paciente.listaVacunas);
}
void _imprimirInorden(TABB A){
    TIPOELEMENTOABB E;
    if(!esAbbVacio(A)){
        _imprimirInorden(izqAbb(A));
        leerElementoAbb(A,&E);
        _imprimirPaciente(E);
        _imprimirInorden(derAbb(A));
    }
}

// Imprimir en fichero. Cada función llama a la anterior
void _imprimirVacunasFichero(TLISTA listaVac, FILE *archivo){
    TIPOELEMENTOLISTA vac;
    for(TPOSICION i = primeroLista(listaVac); i < finLista(listaVac); i = siguienteLista(listaVac, i)){
        recuperarElementoLista(listaVac, i, &vac);
        fprintf(archivo, "%s;%d;%d;", vac.nombreVacuna, vac.nDosis, vac.maxDosis);
    }
    fprintf(archivo, "\n");
}
void _imprimirPacienteFichero(TIPOELEMENTOABB paciente, FILE *archivo){
    fprintf(archivo, "%s|", paciente.ApN);
    _imprimirVacunasFichero(paciente.listaVacunas, archivo);
}
void _imprimirPreordenFichero(TABB A, FILE *archivo){
    TIPOELEMENTOABB E;
    if(!esAbbVacio(A)){
        leerElementoAbb(A,&E);
        _imprimirPacienteFichero(E, archivo);
        _imprimirPreordenFichero(izqAbb(A), archivo);
        _imprimirPreordenFichero(derAbb(A), archivo);
    }
}
void _imprimirPacientesFichero(TABB arbolPacientes, FILE *archivo){
    if(!esAbbVacio(arbolPacientes)) _imprimirPreordenFichero(arbolPacientes, archivo);
}

// Función para quitar el retorno de carro al final de un string
void _strip_line(char *linea){
    linea [strcspn(linea ,"\r\n")] = 0;
}

// PARTE 3 PACVAC. Gestión de colas
// Función para crear la cola de pacientes a partir de un nombre de vacuna
void _meter_e_imprimir_cola_inorden(TABB A, char *vacuna, TCOLA *q, int *contador){

    TIPOELEMENTOABB E;
    TIPOELEMENTOLISTA vac;

    // Buscamos en el árbol de pacientes con un inorden
    if(!esAbbVacio(A)){
        _meter_e_imprimir_cola_inorden(izqAbb(A), vacuna, q, contador);
        leerElementoAbb(A, &E);
        // Iteramos en la lista de vacunas de cada paciente
        for(TPOSICION i = primeroLista(E.listaVacunas); i < finLista(E.listaVacunas); i = siguienteLista(E.listaVacunas, i)){
            recuperarElementoLista(E.listaVacunas, i, &vac);

            // Si encontramos a un paciente que tenga esa vacuna en su lista, lo metemos en la cola
            if(strcmp(vac.nombreVacuna, vacuna) == 0){
                anadirElementoCola(q, E);
                *contador = *contador + 1;
            }
        }
        _meter_e_imprimir_cola_inorden(derAbb(A), vacuna, q, contador);
    }
}

// Función de impresión de una cola
void _imprimirCola(VACUNODROMO vac){

    // Imprimimos la cola, recorriéndola gracias al campo vac.nPacientes que nos da la longitud de la cola
    TIPOELEMENTOCOLA c;
    if(esColaVacia(vac.colaPacientes)) printf("NO HAY MÁS PACIENTES EN LA COLA DE VACUNACIÓN\n");
    else {
        printf("\nVACUNÓDROMO ACTUAL:");
        for (int i = 0; i < vac.nPacientes; i++) {
            consultarPrimerElementoCola(vac.colaPacientes, &c);
            _imprimirPaciente(c);
            suprimirElementoCola(&vac.colaPacientes);
            anadirElementoCola(&vac.colaPacientes, c);
        }
    }
}

// Función que borra un vacunódromo si queremos crear uno nuevo cuando ya hay uno existente
bool _mantener_vacunodromo(VACUNODROMO *V){

    short respuesta;
    if(!esColaVacia((*V).colaPacientes)){
        _imprimirCola(*V);
        printf("¿Desea eliminar el vacunódromo actual?\n1) Sí\n2) No\nRespuesta: ");
        scanf(" %hd", &respuesta);
        if(respuesta == 1) {
            destruirCola(&V->colaPacientes);
            printf("Se ha eliminado el vacunódromo. Ya puedes crear uno nuevo\n\n");
            return true;
        }
        else if (respuesta == 2){
            printf("No se creará otro vacunódromo y permanecerás con tu vacunódromo actual\n");
            return false;
        }
    }
    return true;
}

////////////////////// FUNCIONES VISIBLES EN EL .h ////////////////////
// PARTE 1 Y 2
void anhadirPaciente(TABB *arbolPacientes){

    TIPOELEMENTOABB paciente;
    TIPOELEMENTOLISTA infoVac;

    printf("Introduzca el nombre del paciente: ");
    scanf(" %[^\r\n]", paciente.ApN);

    // Si el paciente ya existe, mandamos un mensaje
    if(esMiembroAbb(*arbolPacientes, paciente))
        printf("Este paciente ya existe en la base de datos\n");

    else{
        crearLista(&paciente.listaVacunas);
        while(true){

            printf("\tIntroduzca el nombre de la vacuna (fin para finalizar): ");
            scanf(" %[^\r\n]", infoVac.nombreVacuna);

            if(strcmp(infoVac.nombreVacuna, "fin") == 0) break;

            printf("\tIntroduzca el número de dosis máximas: ");
            scanf(" %d", &infoVac.maxDosis);

            insertarElementoLista(&paciente.listaVacunas, finLista(paciente.listaVacunas), infoVac);
        }

        insertarElementoAbb(arbolPacientes, paciente);
        printf("\n\nPACIENTE CREADO\n");
    }
}
void eliminarPaciente(TABB *arbolPacientes){

    if(!esAbbVacio(*arbolPacientes)){
        TIPOELEMENTOABB paciente;

        printf("Introduce el nombre del paciente que quieres eliminar: ");
        scanf(" %[^\r\n]", paciente.ApN);

        // Si el paciente se encuentra en la base de datos, lo eliminamos. Si no, mandamos mensaje
        if(esMiembroAbb(*arbolPacientes, paciente)){
            suprimirElementoAbb(arbolPacientes, paciente);
            printf("Se ha eliminado al paciente de la base de datos.\n");
        }
        else printf("No se encuentra a ese paciente en la base de datos\n");
    }
    else printf("\nÁRBOL VACÍO\n");
}
void listadoPacientes(TABB arbolPacientes){
    // Imprimimos toda la lista de pacientes
    if(!esAbbVacio(arbolPacientes)) _imprimirInorden(arbolPacientes);
    else printf("\nLISTA DE PACIENTES VACÍA\n");
}
void cargar_archivo_entrada(TABB *A, int nparam, char **args){

    // Comprobación de parámetros
    if(nparam >= 3 && (strcmp("-f", args[1])==0)){
        FILE *fp = fopen(args[2], "r");

        if (fp != NULL) {
            char linea[100];
            TIPOELEMENTOABB paciente;
            TIPOELEMENTOLISTA infoVac;

            while(fgets(linea, MAXLINEA, fp) != NULL){
                _strip_line(linea);
                // Primeras llamadas con strtok para extraer el nombre del paciente
                char *nombrePaciente = strtok(linea ,"|");
                char *listaVacunas = strtok(NULL, "|") ;
                strcpy(paciente.ApN, nombrePaciente);

                // Ahora cogeremos toda la información que tenga que ver con las vacunas que tenga el paciente
                crearLista(&(paciente.listaVacunas));
                char *token = strtok(listaVacunas,";");
                int n = 0;

                while(token != NULL){
                    n++;
                    switch(n){
                        case 1:
                            strcpy(infoVac.nombreVacuna, token);
                            break;
                        case 2:
                            infoVac.nDosis = atoi(token);
                            break;
                        case 3:
                            infoVac.maxDosis = atoi(token);
                            n = 0;
                            insertarElementoLista(&(paciente.listaVacunas), finLista(paciente.listaVacunas), infoVac);
                            break;
                    }
                    token = strtok(NULL, ";");
                }
                // Insertamos el paciente en el árbol de pacientes
                insertarElementoAbb(A, paciente);
            }
        }
        else printf("\nERROR AL ABRIR EL ARCHIVO\n\n");
        fclose(fp);

    }
    else printf("\nProcesar un fichero con información de pacientes y vacunas.\nFORMATO: $>./ejecutable -f archivo.txt\n");
}
void cargar_archivo_salida(TABB A, char *nombreFichero){
    FILE *fp = fopen(nombreFichero, "w");
    if(!esAbbVacio(A)) _imprimirPacientesFichero(A, fp);
    else fprintf(fp, "NO HAY PACIENTES");
    fclose(fp);
}

// PARTE 3 PACVAC. Gestión de colas
void crear_cola_vacunacion(TABB A, VACUNODROMO *V){

    bool crearNuevoVacunodromo = _mantener_vacunodromo(V);

    // Si hubiese un vacunódromo y no lo quisiesemos mantener, o si no existiese un vacunódromo previo, entonces lo creamos
    if (crearNuevoVacunodromo) {

        printf("\nCREACIÓN DE VACUNÓDROMO\n");
        crearCola(&V->colaPacientes);
        // Inicializamos a 0 la longitud de la cola
        (*V).nPacientes = 0;

        if(!esAbbVacio(A)) {
            printf("Vacuna a administrar: ");
            scanf(" %[^\n\r]", (*V).nombreVacuna);
            _meter_e_imprimir_cola_inorden(A, (*V).nombreVacuna, &V->colaPacientes, &V->nPacientes);

            // Si nuestra cola está vacía, significa que no hay pacientes con esa vacuna; de lo contrario, imprimiremos la cola resultante
            if(esColaVacia((*V).colaPacientes))
                printf("COLA VACÍA. No hay ningún paciente al que haya que suministrarle esa vacuna\n");
            else
                _imprimirCola(*V);
        }
        else if(esAbbVacio(A))
            printf("ÁRBOL VACÍO. No hay pacientes en la base de datos\n");
    }
}
void vacunar(VACUNODROMO *V, TABB *arbolPacientes){

    TIPOELEMENTOCOLA pacienteAVacunar;
    TIPOELEMENTOLISTA vacunaAdministrar;
    bool suprimido = false;

    if(esColaVacia((*V).colaPacientes)) printf("NO HAY UNA COLA DE VACUNACIÓN CREADA.\nPresione la opción C/c del menú del vacunódromo para crear una.\n\n");
    else {

        // Recuperamos el primer paciente de la cola para vacunarlo
        consultarPrimerElementoCola((*V).colaPacientes, &pacienteAVacunar);

        TPOSICION vac;
        // Recorremos las vacunas del paciente para administrarle la correspondiente al vacunódromo
        for(TPOSICION i = primeroLista(pacienteAVacunar.listaVacunas); i < finLista(pacienteAVacunar.listaVacunas); i = siguienteLista(pacienteAVacunar.listaVacunas, i)){
            recuperarElementoLista(pacienteAVacunar.listaVacunas, i, &vacunaAdministrar);

            // Si encontramos la vacuna (que deberíamos si la cola está bien hecha) aumentamos en 1 la dosis del paciente
            if(strcmp(vacunaAdministrar.nombreVacuna, (*V).nombreVacuna) == 0){
                printf("\n- Se vacunará al paciente %s de %s\n", pacienteAVacunar.ApN, (*V).nombreVacuna);
                vacunaAdministrar.nDosis++;
                modificarElementoLista(&pacienteAVacunar.listaVacunas, i, vacunaAdministrar);
                vac = i;
                break;
            }
        }

        // Si después de administrársela, el paciente ya tiene las dosis máximas, le eliminamos esa vacuna
        // de su lista de vacunas, eliminamos al paciente de la cola de vacunación y decrementamos nPacientes
        // en la cola
        if(vacunaAdministrar.nDosis == vacunaAdministrar.maxDosis){
            printf("DOSIS PENDIENTES CUBIERTAS. Se eliminará esta vacuna de la lista del paciente y se eliminará al paciente de la cola\n");
            suprimirElementoLista(&(pacienteAVacunar.listaVacunas), vac);
            suprimirElementoCola(&V->colaPacientes);
            suprimido = true;
            (*V).nPacientes--;

            // Si además, el paciente ya no tiene más vacunas pendientes, lo eliminaremos del árbol
            if (esListaVacia(pacienteAVacunar.listaVacunas)){
                printf("TODAS LAS VACUNAS DEL PACIENTE COMPLETADAS. Se eliminará al paciente de la base de datos\n");
                suprimirElementoAbb(arbolPacientes, pacienteAVacunar);
            }
        }

        // Si el paciente no ha sido suprimido previamente de la cola, la reorganizamos, de manera que el paciente
        // que acabamos de vacunar se vaya al final de la cola para dejar paso al siguiente paciente para vacunar.
        if(!suprimido){
            suprimirElementoCola(&V->colaPacientes);
            anadirElementoCola(&V->colaPacientes, pacienteAVacunar);
        }

        // Imprimimos la cola para ver cómo ha quedado
        _imprimirCola(*V);

        // Modificamos el árbol de pacientes con la nueva información del paciente modificado
        modificarElementoAbb(*arbolPacientes, pacienteAVacunar);

    }
}
