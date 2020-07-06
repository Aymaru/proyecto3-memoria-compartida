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
  
  char shm_name[25];
  
  if (argc >= 2) {  

    for (int i=1;i<argc;i++) {
      if (strcmp(argv[i],"-s") == 0) { //Option Size
        printf("size: %s\n", argv[++i]);


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
  
  printf("hola");

  int md;
  int status;
  long pg_size;
  caddr_t virt_addr;

  /* Create shared memory object */

  md = shm_open (shm_name, O_CREAT|O_RDWR, 0);
  pg_size = 500;

  if((ftruncate(md, pg_size)) == -1){    /* Set the size */
      perror("ftruncate failure");
      exit(1);
  }
                                        
  /* Map one page */

  virt_addr = mmap(0, pg_size, PROT_READ | PROT_WRITE, MAP_SHARED, md, 0);
  
  strcpy(virt_addr,"Escribi en mem compartida\n");
  printf("shm msg: %s\n",virt_addr);

  status = munmap(virt_addr, pg_size);  /* Unmap the page */
  status = close(md);                   /*   Close file   */
  status = shm_unlink(shm_name);     /* Unlink shared-memory object */



  return 0;
}
