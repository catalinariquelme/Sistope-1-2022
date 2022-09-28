/*
            SISTEMAS OPERATIVOS 1/2022 - LABORATORIO 2

Autores:
- Catalina Riquelme Zamora (20.642.812-0)
- Diego Garrido Delgado (20.297.428-7)

Seccion: A-1

Profesor: Fernando Rannou Fuentes

Ayudante: Ricardo Hasbun Miranda


README:
Para compilar este laboratorio se hace uso de un programa makefile. 
Se presenta un ejemplo con los pasos para ejecutar este laboratorio.
    1. make
    2. ./lab2 -i test10000.csv -o salida.txt -d 100 -n 4 -h 3 -c 100 -b
    3. Se recomienda ejecutar "make clean", para eliminar ejecutables.

*/

#include "herramientas.h"
#include <stdio.h>
#include <unistd.h>

FILE * visibilidades; //Descriptor global para archivo de entrada.
TDADisco* discosvis; //Lista con estructuras de discos.
int N;//Cantidad discos.
int chunk; //Numero de lıneas que cada hebra lee
int leido; //Descriptor que permite saber si el archivo fue leido.
float *radius;//Radio

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Entradas: variable sin usar
// Salidas: -
// Objetivo: revisa a que disco pertenece y registra
void *calcularPropiedades(void *unused)
{
    printf(" \n");
    int chunk_aux;
    chunk_aux = chunk;
    if(!feof(visibilidades))
    {
        pthread_mutex_lock(&mutex);
        while(chunk_aux!=0 && leido == 0)
        {
            lecturaYcalculo(leido,chunk,radius,visibilidades,N,discosvis);
            chunk_aux--;
        }
        pthread_mutex_unlock(&mutex);
        calcularPropiedades(NULL); // la hebra vuelve a calcular propiedades hasta que se lea todo el archivo.
    }
    else
    {
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}


// Entradas: int (numero de hebras)
// Salidas: -
// Objetivo: genera h hebras
void generarHebras(int h) 
{
    for (int i = 0; i < h; ++i)
    {
        pthread_t hebra;
        pthread_create(&hebra,NULL, calcularPropiedades,NULL);
        pthread_join(hebra,NULL);
    }
}

int main(int argc, char **argv)
{
    int c;
    int nHebras, ancho, bandera;
    // char* para archivo de entrada y salida
    char *ivalue = NULL;
    char *ovalue = NULL;
    opterr = 0;
    //Ciclo para recibir argumentos por linea de comando
    while ((c = getopt(argc, argv, "i:o:d:n:h:c:b")) != -1)
    {
        switch (c)
        {
            case 'i':
                ivalue = optarg; //archivo entrada
                break;
        
            case 'o':
                ovalue = optarg; //archivo salida
                break;
        
            case 'd':
                sscanf(optarg, "%d", &ancho);
                //Validación de entrada
                if (ancho <= 0)
                {
                    printf("Parametro de entrada(ancho) incorrecto, ingrese un numero mayor a 0.\n");
                    exit(-1);
                }
                break;
                
            case 'n':
                sscanf(optarg, "%d", &N);
                //Validación de entrada
                if (N <= 0)
                {
                    printf("Parametro de entrada(cantidad  de discos) incorrecto, ingrese un numero mayor a 0.\n");
                    exit(-1);
                }
                break;

            case 'h':
                sscanf(optarg, "%d", &nHebras);
                //Validación de entrada
                if (nHebras <= 0)
                {
                    printf("Parametro de entrada(cantidad de hebras) incorrecto, ingrese un numero mayor a 0.\n");
                    exit(-1);
                }
                break;

            case 'c':
                sscanf(optarg, "%d", &chunk);
                //Validación de entrada
                if (chunk <= 0)
                {
                    printf("Parametro de entrada(chunk) incorrecto, ingrese un numero mayor a 0.\n");
                    exit(-1);
                }
                break;
        
            case 'b':
                bandera = 1;
                break;

            case '?':
                //bifurcaciones para cuando no se rellena un dato obligatorio.
                if (optopt == 'c')
                {
                    fprintf(stderr, "Opcion -%c requiere un argumento.\n",optopt);  
                }
                else
                    fprintf(stderr, "Opcion con caracter desconocido `\\x%x'.\n",optopt);
                return 1;

            default:
                abort();
        }
    }

    leido = 0;
    discosvis = crearListaDiscosVacia();
    generarRadios(&radius,N,ancho);
    generarDiscos(N,discosvis); // Estructura-lista de discos 

    visibilidades = fopen(ivalue, "r");
    generarHebras(nHebras);
    fclose(visibilidades);

    //La hebra madre finaliza calculos.
    terminarCalculos(discosvis);
    generarSalida(discosvis,ovalue,bandera,N);

    return 0;
}