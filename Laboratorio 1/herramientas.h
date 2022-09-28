#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define READ 0
#define WRITE 1

struct vsi {
	float u; //Coordenada u
	float v; //Coordenada v
	float i; //Parte imaginaria
	float r; //Parte real
	float w; //Ruido
} typedef vsi;

float distance(float u, float v);

void generarHijos(int ***pipeIn, int ***pipeOut,int N,int flag);

void generarRadios(float ** discos, int N, float ancho);

void enviarInfor(char fi[],int **pipeIn, float *radius,int N);

void escribirTxt(int **pipeOut,int N, char fo[]);

void escribirArchivo(int **salidaPipe,int N, char fo[]);

void obtenerInfo(float *meanReal,float *meanImag,float *potency,float *noise,int *i);

void retornarInfo(float meanReal,float meanImag,float potency,float noise,int i);

void obtenerFlag(int *flag);

int whatSon(float *radius, float distance, int N);

void setVsi(vsi* obj, float u, float v, float r, float i, float w);

int isFin(vsi *obj);

