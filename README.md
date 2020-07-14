# proyecto3-shared-memory
Proyecto 3 del curso de Sistemas Operativos.

## Comandos de compilación
Para construir todos los ejecutables utilice el comando:

    make

Para construir un ejecutable especifico utilice el comando:

    make [creator|finisher|producer|consumer]

Para limpiar los ejecutables de la carpeta use:

    make clear

Si desea compilar manualmente el programa utilice:

    gcc -o creator.o creator.c structs.c -lrt -pthread -lm
    gcc -o finisher.o finisher.c structs.c -lrt -pthread -lm
    gcc -o producer.o producer.c structs.c -lrt -pthread -lm
    gcc -o consumer.o consumer.c structs.c -lrt -pthread -lm

## Comandos de ejecución
Una ejecución normal del programa sigue el siguiente orden:

    ./creator.o -n shm -s 10
    ./producer.o -n shm -t 0.5
    ./consumer.o -n shm -t 0.5
    ./finisher.o -n shm

## Parametros de ejecución    
| Parámetro        | Descripción                                               |
|------------------|-----------------------------------------------------------|
|       -n         | El nombre de la memoria compartida                        |
|       -s         | Cantidad de mensajes del buffer                           |
|       -t         | Media de distribución exponencial para tiempos aleatorios |
