lab1.out: lab1.c herramientas.o vis.out
	gcc lab1.c herramientas.o -o lab1.out -lm

herramientas.o: herramientas.c
	gcc -c herramientas.c -lm

vis.out: vis.c herramientas.o
	gcc herramientas.o vis.c -o vis.out -lm

clean:
	rm lab1.out herramientas.o vis.out
