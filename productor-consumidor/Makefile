## ejecutar en conjunto
cli_exec:
	gcc -o creator.o creator.c structs.c -lrt -pthread
	gcc -o finisher.o finisher.c structs.c -lrt -pthread
	##gcc -o consumidor.o consumidor.c
	##gcc -o finalizador.o finalizador.c


## Compliar y ejecutar creador
creator:
	gcc -o creator.o creator.c structs.c -lrt -pthread

## Compilar y ejecutar finalizador
finisher:
	gcc -o finisher.o finisher.c structs.c -lrt -pthread

## Compilar y ejecutar productor
productor:
	gcc -o productor.o productor.c

## Compilar y ejecutar consumidor
consumidor:
	gcc -o consumidor.o consumidor.c

clean:
	rm -rf *.o *.bin *.elf
