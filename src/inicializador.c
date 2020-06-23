/**
 * Funcion Inicializador del Productor-Consumidor
 * Made by: @gezele14 
 * Miercóles 17 de junio, 2020
*/

//Imports
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include <util.h>
#include <shareMem.h>
#include <semaphore.h>

//Variables globales
char *nombreBuffer = "";
int tamanoBuffer;

//Para la memoria compartida
key_t Clave;
int Id_Memoria;
message *Memoria = NULL;
var *Global = NULL;

//Para los semaforos
int Memc,Lleno,Vacio;

void initializeMem(int tam);

//Funcion Principal
int main(int argc, char *argv[])
{
  system("clear");
  char *dir = "buffers";
  if (!chkDir(dir))
  {
    if (!createDir(dir))
    {
      printc("Error al crear dir\n", 1);
    }
    else
      printc("Dir creado\n", 2);
  }

  //Verifica cantidad de argumentos.
  if (argc != 3)
  {
    printf("Uso correcto:\n\t ./main <Nombre_Buffer> <Tamaño_Buffer>\n");
    exit(0);
  }

  //Llenado de variables globales
  nombreBuffer = argv[1];

  //Verifica que el tamaño del buffer sea un numero
  if (!isNumber(argv[2]))
  {
    printf("Debe digitar un tamaño de buffer válido y entero.\n");
    exit(0);
  }
  tamanoBuffer = atoi(argv[2]);

  //verifica si el directorio existe y si no lo crea
  char *dirName = concat("buffers/", nombreBuffer);
  if (!chkDir(dirName))
  {
    createDir(dirName);
  }

  //Obtension de la clave
  Clave = ftok(dirName, 's');
  if (Clave == -1){
    printf("No se consigue la clave para memoria compartida\n");
    exit(0);
  }

  //Creacion del buffer
  if (createMem(&Id_Memoria, Clave, tamanoBuffer, &Memoria) == 0)
  {
    printf("Error al crear el buffer\n");
    exit(0);
  }

  char test[50];
  sprintf(test, "Se generó el buffer %s de tamaño %d correctamente\n", nombreBuffer, tamanoBuffer);
  printc(test, 6);

  int Id_Variables = 0; 
  if (VarMem(&Id_Variables, &Global) == 0)
  {
    printf("Error al leer las variables globales\n");
    exit(0);
  }

  Global[0].consumers = 0;
  Global[0].producers = 0;
  Global[0].idmem = Id_Memoria;
  Global[0].memSize = tamanoBuffer;
  Global[0].endSys = 0;

  initializeMem(tamanoBuffer);

  //inicializar los semaforos
  Memc=sem_create(dirName,10, 1,1);
  Lleno=sem_create(dirName,11, 1,0);
  Vacio=sem_create(dirName,12, 1,tamanoBuffer);  
  

  int F = 0;
  scanf("%d",&F); 

  if(!deleteMem(Id_Memoria)){
    printc("<I> Error a la hora de eliminar la memoria\n",1);
  }else{
    printc("<I> Memoria limpiada correctamente\n",3);
  }

  if(!deleteMem(Id_Variables)){
    printc("<I> Error a la hora de eliminar la memoria\n",1);
  }else{
    printc("<I> Memoria limpiada correctamente\n",3);
  }
  /* if(!deleteMem(Memc)){
    printc("<I> Error a la hora de eliminar la memoria\n",1);
  }else{
    printc("<I> Memoria limpiada correctamente\n",3);
  } */  
  
  sem_erase(Memc); 
  sem_erase(Lleno);
  sem_erase(Vacio);
  Global[0].endSys = 1;
   
  
  return 0;
}

void initializeMem(int tam)
{
  for (int i = 0; i < tamanoBuffer; i++)
  {
    Memoria[0].used = 0;
    Memoria[0].magic_number = 0;
    strcpy(Memoria[0].date, "");
    strcpy(Memoria[0].hour, "");
    strcpy(Memoria[0].msg, "");
  }
}