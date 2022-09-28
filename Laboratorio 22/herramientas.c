#include "herramientas.h"

//Entrada: -
//Salida: TDADisco
//Objetivo: Crea una lista de dico vacia
TDADisco* crearListaDiscosVacia()
{
	TDADisco* listaDisco=(TDADisco*)malloc(sizeof(TDADisco));
	listaDisco->inicio=NULL;
	return listaDisco;
}


//Entrada: TDADisco (lista discos)
//Salida: 1 (lista vacia) 0 (lista no vacia)
//Objetivo: revisar si lista de discos se encuentra vacia
int esListaDiscosVacia(TDADisco* listaDiscos)
{
	if (listaDiscos->inicio == NULL)
		return 1;
	else
		return 0;
}


//Entrada: TDADisco (lista discos), int (id)
//Salida: -
//Objetivo: Inserta al inicio de la lista un disco
void insertarInicioListaDisco(TDADisco* listaDisco, int id)
{
	disco* nuevo=(disco*)malloc(sizeof(disco));
	nuevo->id=id;
	nuevo->r=0;
	nuevo->i=0;
	nuevo->p=0;
	nuevo->w=0;
	nuevo->cantidad_vis=0;
	nuevo->siguiente = listaDisco->inicio;
	listaDisco->inicio=nuevo;
}


//Entrada: int (numero de discos), TDADIsco (discos)
//Salida: -
//Objetivo: Genera n discos 
void generarDiscos(int N, TDADisco* discos)
{
	for (int i=0; i<N; i++)
	{
		insertarInicioListaDisco(discos,i+1);
	}
}


//Entrada: float ** (discos), int (N), float (ancho)
//Salida: -
//Objetivo: Asigna valores del rango
void generarRadios(float ** discos, int N, float ancho){
	(*discos) = (float*)malloc(sizeof(float)*N);
	for (int i = 0; i < N-1; ++i)
	{
		(*discos)[i] = ancho * (i+1);
	}
	(*discos)[N-1] = 0;
}


//Entradas: float (coordenada u), float (coordenada v)
//Salidas: float (distancia)
//Objetivos: Calcula distancia de la visibilidad
float distance(float u, float v){
	return sqrt(pow(u,2) + pow(v,2));
}


//Entrada: float* (radio), float (distancia), int (N)
//Salida: int (N)
//Objetivo: Determina a que hijo pertenece segun la distancia
int whatSon(float *radio, float distance, int N){
	for(int i = 0; i < N; i ++){
		if(distance < radio[i]){
			return i;
		}
	}
	return N - 1; 
}


//Entrada: TDADisco (discos), disco (disco)
//Salida: -
//Objetivo: Actualizar la visibilidad de la una lista de discos
void actualizarVis(TDADisco* discos,disco* obj)
{
	if (!esListaDiscosVacia(discos))
	{
		disco* auxiliar=discos->inicio;
		while(auxiliar!=NULL)
		{
			if (auxiliar->id == (*obj).id)
			{
				auxiliar->r = auxiliar->r + (*obj).r;
				auxiliar->i = auxiliar->i + (*obj).i;
				auxiliar->p = auxiliar->p + (*obj).p;
				auxiliar->w = auxiliar->w + (*obj).w;
				auxiliar->cantidad_vis = auxiliar->cantidad_vis + 1;
	      	}
	      	auxiliar=auxiliar->siguiente;
		}
	}
}


//Entrada: int (leido), int (chunk),float *(radio), FILE *(visibilidades),int (cantidad discos),TDADisco* (discos)
//Salida: -
//Objetivo: Lectura y actulizacion visibilidad
void lecturaYcalculo(int leido, int chunk,float *radio, FILE *visibilidades,int N,TDADisco* discos) 
{
	if(leido == 0)
	{
		float u,v,r,i,w;
		disco* aux;
		if(fscanf(visibilidades,"%f,%f,%f,%f,%f",&u,&v,&r,&i,&w) != EOF){
			aux->id = whatSon(radio,distance(u,v),N)+1;//1 pertenece al disco 2
			aux->r = r;
			aux->i = i;
			aux->p = sqrt(r*r+i*i);
			aux->w = w;
			actualizarVis(discos,aux);
		}

		// Si llega al final del archvio y faltaron lineas del chunk
		else
		{
			leido = 1;
		}
	}
}


//Entrada: TDADisco (lista discos)
//Salida: -
//Objetivo: Hebra madre finaliza los calculos
void terminarCalculos(TDADisco* listaDiscos)
{
	if (!esListaDiscosVacia(listaDiscos))
	{
		disco* auxiliar=listaDiscos->inicio;
	    while (auxiliar!=NULL)
	    {
			auxiliar->r = auxiliar->r/auxiliar->cantidad_vis;
	        auxiliar->i = auxiliar->i/auxiliar->cantidad_vis;
	        auxiliar=auxiliar->siguiente;
	    }
	    printf("\n");
	}
}


//Entrada: TDADisco* (lista discos), char (fo[]),int (bandera), int (numero de discos)
//Salida: -
//Objetivo: Escribe archivo de salida y muestra por pantalla
void generarSalida(TDADisco* listaDiscos, char fo[],int bandera, int N)
{
	FILE * salida = fopen(fo,"w");
	for(int i = 0; i < N; i++)
	{
		if (!esListaDiscosVacia(listaDiscos))
		{
			disco* auxiliar=listaDiscos->inicio;
			while(auxiliar!=NULL)
			{
				if (auxiliar->id == i+1)
				{
			        fprintf(salida,"Disco %d\n",auxiliar->id); 
			        fprintf(salida,"Media Real: %f\n",auxiliar->r);
			        fprintf(salida,"Media Imaginaria: %f\n",auxiliar->i);
			        fprintf(salida,"Potencia: %f\n",auxiliar->p);
			        fprintf(salida,"Ruido total: %f\n\n",auxiliar->w);

			        if(bandera == 1)
			        {
			        	printf("Disco %d\n",auxiliar->id);
			        	printf("Media Real: %f\n",auxiliar->r);
			        	printf("Media Imaginaria: %f\n",auxiliar->i);
			        	printf("Potencia: %f\n",auxiliar->p);
			        	printf("Ruido total: %f\n\n",auxiliar->w);
			        }
		      	}
		      	auxiliar=auxiliar->siguiente;
			}
		}
	}
	fclose(salida);
}