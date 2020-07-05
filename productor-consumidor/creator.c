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
  
  if (argc >= 2) {

    for (int i=1;i<argc;i++) {
      if (strcmp(argv[i],"-s") == 0) { //Option Size
        printf("size: %s\n", argv[++i]);


      } else if (strcmp(argv[i],"-n") == 0) { //Option Buffer Name
        printf("buffer name: %s\n", argv[++i]);


      } else { //default
        printf("Invalid Option. Use: ./creator.o -s [Cantidad de Mensajes] -n [Nombre del buffer].\n");

      };

    };
  } else {
    printf("No sea puto.!\n"); //Si no lo quitamos, es culpa de garita.!
  };
  



  return 1;
}
