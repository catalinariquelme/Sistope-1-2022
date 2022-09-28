#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

// Estructura donde se registran las propiedades calculadas por cada Disco.
struct disco_vis {
    int id; // Id del disco al que pertenece
    float r; //Sumatoria real
    float i; //Sumatoria imaginaria
    float p; //Sumatoria potencia
    float w; //Sumatoria ruido
    int cantidad_vis; //cantidad de vis calculadas en ese disco.
    struct disco_vis* siguiente;
} typedef disco;

// Lista con propiedades de discos.
typedef struct listaDiscosVis
{
  disco* inicio;
}TDADisco;


TDADisco* crearListaDiscosVacia();
void insertarInicioListaDisco(TDADisco* listaDisco, int id);
int esListaDiscosVacia(TDADisco* listaVacunas);
void generarRadios(float ** discos, int N, float ancho);
void generarDiscos(int N, TDADisco* discos);
float distance(float u, float v);
int whatSon(float *radius, float distance, int N);
void generarHebras(int h);
void lecturaYcalculo(int leido, int chunk,float *radio, FILE *visibilidades,int N,TDADisco* discos);
void actualizarVis(TDADisco* discos,disco* obj);
void terminarCalculos(TDADisco* listaDiscos);
void generarSalida(TDADisco* listaDiscos, char fo[],int bandera, int N);
