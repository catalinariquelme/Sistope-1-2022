/*
            SISTEMAS OPERATIVOS 1/2022 - LABORATORIO 1

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
    2. ./lab1.out -i test10000.csv -o propiedades.txt -d 100 -n 4 -b
    3. Se recomienda ejecutar "make clean", para eliminar ejecutables.
*/



#include "herramientas.h"
#include <stdio.h>
#include <unistd.h>
int main(int argc, char **argv)
{
    int c;
    int nDiscos, ancho, bandera;
    // char* para archivo de entrada y salida
    char *ivalue = NULL;
	char *ovalue = NULL;
    opterr = 0;
    //Ciclo para recibir argumentos por linea de comando
    while ((c = getopt(argc, argv, "i:o:d:n:b")) != -1)
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
                sscanf(optarg, "%d", &nDiscos);
                //Validación de entrada
                if (nDiscos <= 0)
                {
                    printf("Parametro de entrada(cantidad  de discos) incorrecto, ingrese un numero mayor a 0.\n");
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
    
    float *radius;
    int **entradaPipe;
    int **salidaPipe;
	generarHijos(&entradaPipe,&salidaPipe,nDiscos,bandera);

	generarRadios(&radius,nDiscos,ancho);
	
	enviarInfor(ivalue,entradaPipe,radius,nDiscos);

	escribirTxt(salidaPipe,nDiscos,ovalue);
	
	return 0;
}