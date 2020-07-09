## ejecutar en conjunto
cli_exec:
	gcc -o creator.o creator.c structs.c -lrt -pthread
	gcc -o finisher.o finisher.c structs.c -lrt -pthread
	gcc -o producer.o producer.c structs.c -lrt -pthread
	gcc -o consumer.o consumer.c structs.c -lrt -pthread


## Compliar y ejecutar creador
creator:
	gcc -o creator.o creator.c structs.c -lrt -pthread

## Compilar y ejecutar finalizador
finisher:
	gcc -o finisher.o finisher.c structs.c -lrt -pthread

## Compilar y ejecutar productor
producer:
	gcc -o producer.o producer.c structs.c -lrt -pthread

## Compilar y ejecutar consumidor
consumer:
	gcc -o consumer.o consumer.c structs.c -lrt -pthread

clean:
	rm -rf *.o *.bin *.elf
