#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <string.h>

#include <util.h>
#include <shareMem.h>
#include <semaphore.h>


message *buffer= NULL;
var *Variables = NULL;
key_t bufferKey;

int Memc,Lleno,Vacio;

int main(int argc, char *argv[]){

  system("clear");

  if(argc != 2){
    printc("<E> El uso correcto es:\n \t./finalizador <Nombre_Buffer>\n",1);
    exit(0);
  }

  //Creacion de la clave para el buffer
  char * buffdir = concat("buffers/",argv[1]);
  if (!chkDir(buffdir)) {
    printc("<E> El buffer no existe\n",1);
    exit(0);
  }else{
    bufferKey =  ftok (buffdir, 's');
    if (bufferKey == -1){
      printc("<E> Error al consiguir la clave para memoria compartida\n",1);
      exit(0);
    }
  }

  //obtencion de las variables globales.
  int idVar = 0;
  if (VarMem(&idVar,&Variables) == 0){
    printc("<E> Error al leer las variables globales\n",1);
    exit(0);
  }

  printc("<I> Variables Globales cargadas correctamente\n",3);
  sleep(1);

  int idmem;
  if(!createMem(&idmem, bufferKey, Variables[0].memSize,&buffer)){
    printc("<E> Error al obtener el buffer\n",1);
    exit(0);
  }

  if(!(idmem == Variables[0].idmem)){
    printc("<E> El Buffer aun no se a inicializado\n",1);
    exit(0);
  }

  printc("<I> Buffer cargado correctamente\n",3);
  sleep(1);

  while (Variables[0].consumers > 0){
    Variables[0].endSys = 1;
    usleep(500000);
  }

  printc("<I> Todods los consumidores estan cerrados\n",3);
  sleep(1);

  while (Variables[0].producers > 0){
    Variables[0].endSys = 2;
    usleep(500000);
  }

  printc("<I> Todods los consumidores estan cerrados\n",3);
  sleep(1);

  if(!deleteMem(idmem)){
    printc("<I> Error al liberar el bufer\n",1);
    exit(0);
  }else{
    printc("<I> El buffer fue liberado correctamente\n",3);
    sleep(1);
  }

  if(!deleteMem(idVar)){
    printc("<I> Error a la hora de eliminar las Variables globales\n",1);
    exit(0);
  }else{
    printc("<I> Las Variables Globales fueron liberado correctamente\n",3);
    sleep(1);
  }

  Memc = sem_open(buffdir,10,1);
  Lleno = sem_open(buffdir,11,1);
  Vacio = sem_open(buffdir,12,1);

  sem_erase(Memc); 
  sem_erase(Lleno);
  sem_erase(Vacio);

  printc("<I> Resumen de actividad: \n\n",4);
  printc("\t- Mensajes Totales: ",4);
  printf("%d\n", Variables[0].totalMsg);
  printc("\t- Mensajes en el buffer: ",4);
  printf("%d\n", msgInMem(buffer, Variables[0].memSize));
  printc("\t- Total de Productores: ",4);
  printf("%d\n", Variables[0].Tproducers);
  printc("\t- Total de Consumidores: ",4);
  printf("%d\n", Variables[0].Tconsumers);
  printc("\t- Consumidores eliminados por llave: ",4);
  printf("%d\n", Variables[0].keyEliminated);
  printc("\t- Tiempo esperado total: ",4);
  printf("%.2f minutos\n", Variables[0].totalWait/60);
  printc("\t- Tiempo bloqueado total: ",4);
  printf("%.2f minutos\n", Variables[0].totalBloq/60);
  printc("\t- Tiempo usuario total: ",4);
  printf("%.2f minutos\n", Variables[0].totalUser/60);
  printc("\t- Tiempo kernel total: ",4);
  printf("%.2f minutos\n", Variables[0].totalKernel/60);

  return 0;
}