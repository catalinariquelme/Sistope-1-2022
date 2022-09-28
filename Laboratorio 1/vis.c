#include "herramientas.h"

int main(int argc, char const *argv[])
{
	int flag;
	float meanReal = 0.0, meanImag = 0.0, potency = 0.0, noise = 0.0;
	int i = 0;
	obtenerFlag(&flag);

	obtenerInfo(&meanReal,&meanImag,&potency,&noise,&i);
	
	retornarInfo(meanReal,meanImag, potency, noise, i);

	if(flag == 1){
		freopen("/dev/tty","a",stdout);
		printf("Soy el hijo de pid %d, procese %d visibilidades\n",getpid(), i);	
	}
	return 0;
}