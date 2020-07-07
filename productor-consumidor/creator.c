#include "creator.h"

// void* create_shared_memory(size_t size) {
//   // Our memory buffer will be readable and writable:
//   int protection = PROT_READ | PROT_WRITE;

//   // The buffer will be shared (meaning other processes can access it), but
//   // anonymous (meaning third-party processes cannot obtain an address for it),
//   // so only this process and its children will be able to use it:
//   int visibility = MAP_SHARED;

//   // The remaining parameters to `mmap()` are not important for this use case,
//   // but the manpage for `mmap` explains their purpose.
//   return mmap(NULL, size, protection, visibility, -1, 0);
// }

/*--------
  GLOBAL
--------*/

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
        i++;
        printf("size: %s\n", argv[i]);
        array_size = atoi(argv[i]);


      } else if (strcmp(argv[i],"-n") == 0) { //Option Buffer Name
        i++;
        printf("buffer name: %s\n", argv[i]);
        strcpy(shm_name, argv[i]);

      } else { //default
        printf("Invalid Option. Use: ./creator.o -s [Cantidad de Mensajes] -n [Nombre del buffer].\n");

      };

    };
  } else {
    printf("No sea puto.!\n"); //Si no lo quitamos, es culpa de garita.!
  };
  
  buff = init_buffer(array_size);
  
  /* Create shared memory object */

  fd = shm_open (shm_name, O_CREAT|O_RDWR, 0);
  
  if((ftruncate(fd, sizeof(struct buffer_t) + (sizeof(struct message_t) * array_size) )) == -1) {    /* Set the size */
      perror("ftruncate failure");
      exit(1);
  }
                                        
  /* Map one page */

  shm_buffer = mmap(0, sizeof(struct buffer_t) + (sizeof(struct message_t) * array_size),
                    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  
  memcpy(shm_buffer,buff,sizeof(struct buffer_t) + (sizeof(struct message_t) * array_size));
  free_buffer(buff);
  
  // pruebas con la memoria compartida, borrar este segmento.
  struct message_t * tmp_msg;
  int msg_index;
  printf("Cantidad max de mensajes: %d\n",shm_buffer->array_size);

  print_buffer_status(shm_buffer);

  insert_msg(shm_buffer,1,2);
  insert_msg(shm_buffer,2,1);
  insert_msg(shm_buffer,3,4);
  insert_msg(shm_buffer,4,0);

  print_buffer_status(shm_buffer);

  tmp_msg = get_msg(shm_buffer,&msg_index);
  print_message(tmp_msg,msg_index);
  free(tmp_msg);

  print_buffer_status(shm_buffer);

  tmp_msg = get_msg(shm_buffer,&msg_index);
  print_message(tmp_msg,msg_index);
  free(tmp_msg);

  print_buffer_status(shm_buffer);

  tmp_msg = get_msg(shm_buffer,&msg_index);
  print_message(tmp_msg,msg_index);
  free(tmp_msg);

  print_buffer_status(shm_buffer);

  insert_msg(shm_buffer,11,4);
  insert_msg(shm_buffer,15,3);
  insert_msg(shm_buffer,13,2);
  insert_msg(shm_buffer,14,1);

  print_buffer_status(shm_buffer);

  tmp_msg = get_msg(shm_buffer,&msg_index);
  print_message(tmp_msg,msg_index);
  free(tmp_msg);

  print_buffer_status(shm_buffer);

  tmp_msg = get_msg(shm_buffer,&msg_index);
  print_message(tmp_msg,msg_index);
  free(tmp_msg);

  print_buffer_status(shm_buffer);

  tmp_msg = get_msg(shm_buffer,&msg_index);
  print_message(tmp_msg,msg_index);
  free(tmp_msg);

  print_buffer_status(shm_buffer);

  insert_msg(shm_buffer,27,0);
  insert_msg(shm_buffer,28,3);
  insert_msg(shm_buffer,29,2);

  print_buffer_status(shm_buffer);

  tmp_msg = get_msg(shm_buffer,&msg_index);
  print_message(tmp_msg,msg_index);
  free(tmp_msg);

  print_buffer_status(shm_buffer);

  tmp_msg = get_msg(shm_buffer,&msg_index);
  print_message(tmp_msg,msg_index);
  free(tmp_msg);

  print_buffer_status(shm_buffer);

  tmp_msg = get_msg(shm_buffer,&msg_index);
  print_message(tmp_msg,msg_index);
  free(tmp_msg);

  print_buffer_status(shm_buffer);

  tmp_msg = get_msg(shm_buffer,&msg_index);
  print_message(tmp_msg,msg_index);
  free(tmp_msg);

  print_buffer_status(shm_buffer);

  tmp_msg = get_msg(shm_buffer,&msg_index);
  print_message(tmp_msg,msg_index);
  free(tmp_msg);

  print_buffer_status(shm_buffer);

  
  // fin pruebas

  close(fd);                   /*   Close file   */
  shm_unlink(shm_name);     /* Unlink shared-memory object */



  return 0;
}
