#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <shareMem.h>

int main(int argc, char *argv[]){
  key_t key;
  var *Variables = NULL;

  int id = atoi(argv[1]);

  if(!deleteMem(id)){
    printf("Error al liberar memoria\n");
  }
}