#include "finisher.h"

/*--------
  GLOBAL
--------*/



/*--------------------------
       MAIN FINALIZADOR
--------------------------*/
int main(int argc, char *argv[]) {
  char shm_name[25]; //Nombre del segmento de memoria compartida.
  struct buffer_t * buff,* shm_buffer; //Puntero para inicializar el buffer y puntero para mapear la memoria compartida usando mmap.
  int fd; //File descriptor de la memoria compartida
  int * array_size;
  size_t size;
  
  if (argc >= 2) { 
    for (int i=1;i<argc;i++) {
      if (strcmp(argv[i],"-n") == 0) { //Option Buffer Name
        i++;
        strcpy(shm_name, argv[i]);
      } else { //default
        printf("Invalid Option. Use: ./finisher.o -n [Nombre del buffer].\n");
      };
    };
  } else {
    printf("No sea puto.!\n"); //Si no lo quitamos, es culpa de garita.!
  };
  
  /* Open shared memory object */
  struct message_t * tmp_msg;
  int index;
  printf("antes.\n");
  if ((fd = shm_open (shm_name, O_CREAT|O_EXCL, 0)) == -1) {
    int errsv = errno;
    if (errsv == EEXIST) {
      printf("Si existe\n");
      if ((fd = shm_open (shm_name, O_RDWR, 0)) == -1) { 
        printf("error %s ",strerror(errno));
        printf("Error opening shared-memory file descriptor.\n");
        exit(1);
      };
  
    };
  };
  printf("despues de shm open.\n");                   
  /* Map one page */
  array_size = mmap(0,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  printf("despues primer mmap.\n");
  size = *array_size;

  printf("declaracion size after.\n");
  munmap(array_size, sizeof(size_t));
  printf("despues munmap.\n");
  shm_buffer = mmap(0, sizeof(struct buffer_t) + (sizeof(struct message_t) * size),
                    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  printf("despues segundo mmap.\n");
  //sem_wait(&shm_buffer->sem_buffer); //Lock the buffer sem
  //Critical Region
  print_buffer_status(shm_buffer);
  shm_buffer->end = 1;
  tmp_msg = get_msg(shm_buffer,&index);
  print_message(tmp_msg,index);
  free(tmp_msg);
  //End of critical Region
  //sem_post(&shm_buffer->sem_buffer); //Unlock the buffer sem

  close(fd); //Close shared-memory file descriptor
  shm_unlink(shm_name); //Unlink shared-memory object 
  
  return 1;
}
