cli_exec:
	gcc -o creador.o creador.c
	gcc -o productor.o productor.c
	gcc -o consumidor.o consumidor.c
	gcc -o finalizador.o finalizador.c
	## ejecutar en conjunto

creator:
	gcc -o creator.o creator.c -lrt -pthread
	##./creator.o
	## Compliar y ejecutar creador

productor:
	gcc -o productor.o productor.c
	## Compilar y ejecutar productor

consumidor:
	gcc -o consumidor.o consumidor.c
	## Compilar y ejecutar consumidor

finalizador:
	gcc -o finalizador.o finalizador.c
	## Compilar y ejecutar finalizador

clean:
	rm -rf *.o *.bin *.elf
