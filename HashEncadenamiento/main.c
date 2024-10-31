#include <stdio.h>
#include <stdlib.h>
#include "tabla_hash_encadenamiento.h"

//Hay que añadir como parámetro la variable nColisionesI: número de colisiones en inserción
void insercionArchivo(FILE *fp, TablaHash *t, unsigned int tipo, unsigned int K, int *nColisionesI);
//Hay que añadir como parámetro la variable nPasosExtraB: número de pasos extra en búsqueda
void busquedaArchivo(FILE *fp, TablaHash t, unsigned int tipo, unsigned int K, int *nPasosExtraB);

int main() {

    unsigned int tipoFH; //tipo de función hash: 1-2-3
    unsigned int K = 256; //factor de ponderación para hash por suma ponderada

    TablaHash t; //tabla hash

    //Inicializamos nColisionesI y nPasosExtraB a 0
    int nColisionesI = 0;
    int nPasosExtraB = 0;

    //Abrimos el fichero de datos de ejemplo, con 10000 jugadores
    FILE *fp = fopen("pacientes_vacunodromo.txt", "rt");

    //Informo del tamaño de la tabla actual
    printf("Tamaño de la tabla hash: %d\n", N);
    
    //Pregunto al usuario que función hash quiere utilizar
    printf("Función hash a utilizar:"
            "\n\t1: método división (suma ascii módulo N)"
            "\n\t2: suma ponderada con K = 256"
            "\n\t3: suma ponderada con otro valor de K ");
    printf("\nTipo función hash: "); scanf("%d", &tipoFH);

    if(tipoFH == 3) {
        printf("\tValor de ponderación K (256): ");
        scanf("%d",&K);
    }
    
    printf("\nFUNCIÓN HASH %u", tipoFH);
    if (tipoFH == 2 || tipoFH == 3) printf(" con K = %u\n",K);

    //Inicializo la tabla hash por encadenamiento
    InicializarTablaHash(t);

    //Llamamos a la función de inserción en t a partir del archivo y el tipo de función hash a utilizar
    printf("\n\t----INSERCIÓN ENCADENAMIENTO----\n");
    insercionArchivo(fp, &t, tipoFH, K, &nColisionesI);
    printf("Número de colisiones: %d", nColisionesI);
   
    rewind(fp); //rebobino

    //Llamo a la función de búsqueda en t a partir del archivo y el tipo de función hash a utilizar
    printf("\n\n\t----BÚSQUEDA ENCADENAMIENTO----\n");
    busquedaArchivo(fp, t, tipoFH, K, &nPasosExtraB);
    printf("Número de pasos extra: %d\n\n", nPasosExtraB);
    
    DestruirTablaHash(t); //Destruyo la tabla
    fclose(fp); //Cierro el archivo    

    printf("FINALIZA EL PROGRAMA\n");

    return (EXIT_SUCCESS);
}

//Función para insertar usuarios en tabla hash, usando la función hash "tipo"
void insercionArchivo(FILE *fp, TablaHash *t, unsigned int tipoFH, unsigned int K, int *nColisionesI) {

    TIPOELEMENTO paciente;

    if (fp) {
        fscanf(fp, " %[^-] - %s - %s", paciente.nombre, paciente.alias, paciente.correo);
        while (!feof(fp)) {
            int hayColision = InsertarHash(t, paciente, tipoFH, K); 
            if (hayColision) (*nColisionesI)++;

            fscanf(fp, " %[^-] - %s - %s", paciente.nombre, paciente.alias, paciente.correo);
        }
    }
    else printf("El archivo no ha podido abrirse\n");
}

//Función para buscar un elemento leído del archivo en la tabla t, usando la función hash "tipo"
void busquedaArchivo(FILE *fp, TablaHash t, unsigned int tipoFH, unsigned int K, int *nPasosExtraB) {

    TIPOELEMENTO paciente;

    if (fp) {
        fscanf(fp, " %[^-] - %s - %s", paciente.nombre, paciente.alias, paciente.correo);
        while (!feof(fp)) {
            BuscarHash(t, paciente.correo, &paciente, tipoFH, K, nPasosExtraB);
            fscanf(fp, " %[^-] - %s - %s", paciente.nombre, paciente.alias, paciente.correo);
        }
    }
    else printf("El archivo no ha podido abrirse\n");
}