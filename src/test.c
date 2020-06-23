#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <shareMem.h>

int main()
{
  key_t key;
  int id_memory;
  var *Variables = NULL;
  message *memory = NULL;

  key = ftok("/bin/ls", 33);
  if (key == -1){
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  if (VarMem(&Variables) == 0){
    printf("Error al leer las variables globales\n");
    exit(0);
  }
  printf("Consd: %d \n", Variables[0].idmem);

  if(getMem(&memory,Variables[0].idmem) == 0){
    printf("Error al leer la memoria\n");
    exit(0);
  }

    printf("Read data1 = %d\n", memory[1025].pid);
    printf("Read data2 = %d\n", memory[1025].magic_number);

  if (id_memory != -1)
  {
    shmdt((char *)memory);
  }

  return 0;
}