
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
  int Tproducers;
  int Tconsumers;
  int producers;
  int consumers;
  int keyEliminated;
  double totalWait;
  double totalBloq;
  double totalUser;
  double totalKernel;
  int readIndex;
  int writeIndex;
  int idmem;
  int totalMsg;
  int memSize;
  int endSys;
} var;


int createMem(int *shmid, key_t key, int size, message **Memoria);
int VarMem(int *shmid, var **Memoria);
int getMem(message **Memoria, int idMem);
int deleteMem(int shmid);
int isEmpty(message *Memoria, int tam);
int isFull(message *Memoria, int tam);
int freeMemSpace(message *Memoria, int tam, int index);
int usedMemSpace(message *Memoria, int tam, int index);
int msgInMem(message *Memoria, int tam);

#endif 