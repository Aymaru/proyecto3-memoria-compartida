cli_exec:
	gcc -o creador.o creador.c
	gcc -o productor.o productor.c
	gcc -o consumidor.o consumidor.c
	gcc -o finalizador.o finalizador.c
	## ejecutar en conjunto

creator:
	gcc -o creator.o creator.c structs.c -lrt -pthread
	##./creator.o
	## Compliar y ejecutar creador

finisher:
	gcc -o finisher.o finisher.c structs.c -lrt -pthread
	## Compilar y ejecutar finalizador

productor:
	gcc -o productor.o productor.c
	## Compilar y ejecutar productor

consumidor:
	gcc -o consumidor.o consumidor.c
	## Compilar y ejecutar consumidor

clean:
	rm -rf *.o *.bin *.elf
