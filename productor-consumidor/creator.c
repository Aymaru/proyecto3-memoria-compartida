#include "creator.h"

/*--------------------------
       MAIN CREADOR
--------------------------*/
int main(int argc, char *argv[]) {

  char shm_name[25]; //Nombre del segmento de memoria compartida.
  struct buffer_t * buff,* shm_buffer; //Puntero para inicializar el buffer y puntero para mapear la memoria compartida usando mmap.
  size_t array_size; //Cantidad de mensajes del buffer
  int fd; //File descriptor de la memoria compartida

  if (argc >= 2) {

    for (int i=1;i<argc;i++) {
      if (strcmp(argv[i],"-s") == 0) { //Option Size
        if (argv[++i] == NULL || (array_size = atoi(argv[i])) == 0){
          printf("Invalid size option. Please input an integer greater than 0.\n");
          exit(EXIT_FAILURE);
        }
      } else if (strcmp(argv[i],"-n") == 0) { //Option Buffer Name
        if (argv[++i] == NULL){
          printf("Invalid buffer name. Please input a valid buffer name.\n");
          exit(EXIT_FAILURE);
        }
        strcpy(shm_name, argv[i]);
      } else { //default
        printf("Invalid Option. Use: ./creator.o -s [Cantidad de Mensajes] -n [Nombre del buffer].\n");
        exit(EXIT_FAILURE);
      };
    };
  } else {
    printf("Invalid option. Use: ./creator.o -s [Cantidad de Mensajes] -n [Nombre del buffer].\n");
    exit(EXIT_FAILURE);
  };

  buff = init_buffer(array_size); //Initialize buffer.

  if ((fd = shm_open (shm_name, O_CREAT|O_EXCL|O_RDWR, 0600)) == -1) { //Create shared-memory object.
      int errsv = errno;
      if (errsv == EEXIST) {
        printf("Error creating shared-memory object. Already exists.\n");
        exit(EXIT_FAILURE);
      } else {
        printf("Error creating shared-memory object.\n");
        perror("ERROR");
        exit(EXIT_FAILURE);
      }
  }
  //Truncate to set the size of the shared-memory object
  if((ftruncate(fd, sizeof(struct buffer_t) + (sizeof(struct message_t) * array_size) )) == -1) {
      perror("ftruncate failure");
      exit(EXIT_FAILURE);
  }

  //Map the shared-memory object to the process.
  shm_buffer = mmap(0, sizeof(struct buffer_t) + (sizeof(struct message_t) * array_size),
                    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  
  //Copy the buffer to the shared memory
  memcpy(shm_buffer,buff,sizeof(struct buffer_t) + (sizeof(struct message_t) * array_size));
  free_buffer(buff);

  close(fd);

  exit(EXIT_SUCCESS);
}
