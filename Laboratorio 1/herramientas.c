#include "herramientas.h"

//Entradas: float (coordenada u), float (coordenada v)
//Salidas: float (distancia)
//Objetivos: Calcula distancia de la visibilidad
float distance(float u, float v){
	return sqrt(pow(u,2) + pow(v,2));
}

//Entrada: int *** (entrada pipe), int *** (salida pipe) , int (numero discos), int (bandera)
//Salida: -
//Objetivo: Genera hijos del proceso del proceso padre
void generarHijos(int ***entradaPipe, int ***salidaPipe,int N,int flag){
	(*entradaPipe) = (int**)malloc(sizeof(int)*N);
	(*salidaPipe) = (int**)malloc(sizeof(int)*N);
	int pid = getpid(), posicion;

	for (int i = 0; i < N; ++i)
	{
		posicion = i;
		(*entradaPipe)[i] = (int*)malloc(sizeof(int)*2);
		(*salidaPipe)[i] = (int*)malloc(sizeof(int)*2);
		pipe((*entradaPipe)[i]);
		pipe((*salidaPipe)[i]);
		
		if(fork() == 0){
			break;
		}
	}
	char *args[] = {"vis.out",NULL};
	if(getpid() != pid){
		dup2((*entradaPipe)[posicion][READ], STDIN_FILENO);
		dup2((*salidaPipe)[posicion][WRITE], STDOUT_FILENO);
		execv("vis.out",args);
	}

	for(int i = 0; i < N; i ++){
		write((*entradaPipe)[i][WRITE], &flag, sizeof(int));
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

//Entrada: char (archivo de entrada),int** (entrada pipe), float * (radio), int (numero de discos)
//Salida: -
//Objetivo: Registra valores y calcula la distancia
void enviarInfor(char fi[],int **entradaPipe, float *radio, int N){
	FILE *visibilidades = fopen(fi, "r");
	if(visibilidades == NULL){
		printf("Error!");
		exit(1);
	}
	float u,v,r,i,w;
	vsi aux;
	while(fscanf(visibilidades,"%f,%f,%f,%f,%f",&u,&v,&r,&i,&w) != EOF){
		setVsi(&aux, u,v,r,i,w);
		write(entradaPipe[whatSon(radio, distance(u,v),N)][WRITE],&aux, sizeof(vsi));
	}
	fclose(visibilidades);
	setVsi(&aux, 0,0,0,0,0);//FIN
	for(int i = 0; i < N; i ++){
		write(entradaPipe[i][WRITE],&aux, sizeof(vsi));	
	}
}

//Entrada: int ** (salida pipe) ,int (numero de discos), char (archivo de salida)
//Salida: -
//Objetivo: Muestra información de propiedades
void escribirTxt(int **salidaPipe,int N, char fo[]){
	vsi aux;
	FILE * salida = fopen(fo,"w");
	for(int i = 0; i < N; i++){
		read(salidaPipe[i][READ],&aux,sizeof(vsi));
		
        fprintf(salida,"Disco %d\n",i+1); 
        fprintf(salida,"Media Real: %f\n",aux.u);
        fprintf(salida,"Media Imaginaria: %f\n",aux.v);
        fprintf(salida,"Potencia: %f\n",aux.r);
        fprintf(salida,"Ruido total: %f\n\n",aux.i);
	}
}

//Entrada: float* (parte real), float* (parte imaginaria), float* (potencia), float* (ruido),int* (i)
//Salida: -
//Objetivo: escribe archivo de información de propiedades
void obtenerInfo(float *meanReal,float *meanImag,float *potency,float *noise,int *i){
	vsi aux;
	while(1){
		read(STDIN_FILENO, &aux, sizeof(aux));
		if(isFin(&aux)){
			break;
		}else{
			(*i) ++;
			(*meanReal) += aux.r;
			(*meanImag) += aux.i;
			(*potency) += sqrt(aux.r*aux.r+aux.i*aux.i);
			(*noise) += aux.w;
		}
	}
}

//Entrada: float (parte real), float (parte imaginaria),float (potencia), float (ruido), int (i)
//Salida: -
//Objetivo: Almacena media ral, imaginaria, potencia y ruido total en la estructura
void retornarInfo(float meanReal,float meanImag,float potency,float noise,int i){
	vsi aux;
	if(i != 0){
		//float u -> media real
		//float v -> media imaginaria
		//float i -> ruido total
		//float r -> potencia
		setVsi(&aux,meanReal/i,meanImag/i,potency,noise,0.0);
	}else{
		setVsi(&aux,0,0,0,0,0);
	}
	write(STDOUT_FILENO, &aux,sizeof(vsi));
}

//Entrada: int* (bandera)
//Salida: -
//Objetivo: Obtener bandera
void obtenerFlag(int *flag){
	read(STDIN_FILENO,flag,sizeof(int));
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

//Entrada: vsi* (visibilidad auxiliar),float (coordenada u), float (coordenada v), float (parte real), float (parte imaginaria), float (ruido)
//Salida: -
//Objetivo: Ingresa datos a estructura
void setVsi(vsi* obj, float u, float v, float r, float i, float w){
	(*obj).u = u;
	(*obj).v = v;
	(*obj).r = r;
	(*obj).i = i;
	(*obj).w = w;
}

//Entrada: vsi* (visibilidad auxiliar)
//Salida: int
//Objetivo: Determina cuando los hijos deben parar de obtener informaciòn
int isFin(vsi *obj){
	return (*obj).u == 0 && (*obj).v == 0 && (*obj).r == 0 && (*obj).i == 0 && (*obj).w == 0;
}