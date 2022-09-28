lab2.out: lab2.c herramientas.o
	gcc lab2.c herramientas.o -o lab2 -lm -pthread

herramientas.o: herramientas.c
	gcc -c herramientas.c -lm -pthread

clean:
	rm lab2 herramientas.o
