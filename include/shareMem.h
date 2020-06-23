
#ifndef _SHAREMEM_H
#define _SHAREMEM_H

typedef struct
{
  int used;
  int magic_number;
  char date[50];
  char hour[50];
  char msg[50];
} message;

typedef struct
{
  int producers;
  int consumers;
  int idmem;
  int memSize;
  int endSys;
} var;


int createMem(int *shmid, key_t key, int size, message **Memoria);
int VarMem(int *shmid, var **Memoria);
int getMem(message **Memoria, int idMem);
int deleteMem(int shmid);
int isEmpty(message *Memoria, int tam);
int isFull(message *Memoria, int tam);
int freeMemSpace(message *Memoria, int tam);
int usedMemSpace(message *Memoria, int tam);

#endif 