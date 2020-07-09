## ejecutar en conjunto
cli_exec:
	gcc -o creator.o creator.c structs.c -lrt -pthread -lm
	gcc -o finisher.o finisher.c structs.c -lrt -pthread -lm
	gcc -o producer.o producer.c structs.c -lrt -pthread -lm
	gcc -o consumer.o consumer.c structs.c -lrt -pthread -lm


## Compliar y ejecutar creador
creator:
	gcc -o creator.o creator.c structs.c -lrt -pthread -lm

## Compilar y ejecutar finalizador
finisher:
	gcc -o finisher.o finisher.c structs.c -lrt -pthread -lm

## Compilar y ejecutar productor
producer:
	gcc -o producer.o producer.c structs.c -lrt -pthread -lm

## Compilar y ejecutar consumidor
consumer:
	gcc -o consumer.o consumer.c structs.c -lrt -pthread -lm

clean:
	rm -rf *.o *.bin *.elf
