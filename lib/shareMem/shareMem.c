
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../../include/shareMem.h"

#define ARRAY_SIZE   4000
#define MALLOC_SIZE 10000
#define SHM_MODE    (SHM_R | SHM_W) 

/**
 * Creacion del buffer de memoria compartida
 * Retorna 0 si hubo algun problema
 * Retorna 1 si todo funciono
*/
int createMem(int *shmid, key_t key, int size, message **Memoria){
  if ((*shmid=shmget(key, sizeof(message)*size, 0777 | IPC_CREAT))<0) {
    printf("Error en el get del buffer\n");
    return 0;
   } 
   if ((*Memoria=( message *)shmat(*shmid, (char *)0, 0)) == (void *) -1) {
    printf("Error en el mat del buffer\n");
    return 0;
   }
   return 1;
}

/**
 * Creacion del buffer de variables globales
 * Retorna 0 si hubo algun problema
 * Retorna 1 si todo funciono
*/
int VarMem(int *shmid, var **Memoria){
  key_t key = ftok ("/bin/ls", 33);
	if (key == -1){
		printf("No se consigue la clave para las variables globales\n");
		return 0;
	}
  if ((*shmid=shmget(key, sizeof(var), 0777 | IPC_CREAT))<0) {
    printf("Error en el get del buffer\n");
    return 0;
   } 
   if ((*Memoria=( var *)shmat(*shmid, (char *)0, 0)) == (void *) -1) {
    printf("Error en el mat del buffer\n");
    return 0;
   }
   return 1;
}

/**
 * Obtension de un buffer a partir de un id
 * Retorna 0 si hubo algun problema
 * Retorna 1 si todo funciono
*/
int getMem(message **Memoria, int idMem){
   if ((*Memoria=( message *)shmat(idMem, (char *)0, 0)) == (void *) -1) {
    printf("Error en el mat del buffer\n");
    return 0;
   }
   return 1;
}

/**
 * Libera la memoria compartida.
 * Retorna 0 si hubo algun problema
 * Retorna 1 si todo funciono
*/
int deleteMem(int shmid){
  if (shmctl(shmid, IPC_RMID, 0) < 0) {
    return 0;
  } else
    return 1;
}

/**
 * Verifica si la memoria esta vacia
 * Retorna 0 no esta vacia
 * Retorna 1 vacia
*/
int isEmpty(message *Memoria, int tam){
  for (int i = 0; i < tam; i++){
    if(Memoria[i].used){
      return 0;
    }
  }
  return 1;
}

/**
 * Verifica si la memoria esta llena
 * Retorna 0 no esta vacia
 * Retorna 1 vacia
*/
int isFull(message *Memoria, int tam){
  for (int i = 0; i < tam; i++){
    if(!(Memoria[i].used)){
      return 0;
    }
  }
  return 1;
}

/**
 * Retorna el valor del indece donde hay espacio
 * si no encuentra devuelve -1
 * 
*/
int freeMemSpace(message *Memoria, int tam){
  for (int i = 0; i < tam; i++){
    if(!(Memoria[i].used)){
      return i;
    }
  }
  return -1;
}

/**
 * Retorna el valor del indece donde hay espacio
 * si no encuentra devuelve -1
 * 
*/
int usedMemSpace(message *Memoria, int tam){
  for (int i = 0; i < tam; i++){
    if((Memoria[i].used)){
      return i;
    }
  }
  return -1;
}
