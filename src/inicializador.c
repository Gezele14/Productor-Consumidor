/**
 * Basado en el Tutorial de:
 * C.U. Elena y S.R Ranquel
 * URL: https://www2.infor.uva.es/~cllamas/concurr/pract97/rsantos/index.html
 * 
 * chuidiang.org
 * URL: http://www.chuidiang.org/clinux/ipcs/mem_comp.php
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
int Memc, Lleno, Vacio;

void initializeMem(int tam);

//Funcion Principal
int main(int argc, char *argv[])
{
  system("clear");
  printc("Inicializador | Productor-Consumidor\n\n", 6);
  char *dir = "buffers";
  if (!chkDir(dir))
  {
    if (!createDir(dir))
    {
      printc("<E> Error al crear dir\n", 1);
    }
    else
      printc("<i> Dir de buffers creado\n", 2);
  }

  //Verifica cantidad de argumentos.
  if (argc != 3)
  {
    printc("<E> Uso correcto:\n\t ./main <Nombre_Buffer> <Tamaño_Buffer>\n", 1);
    exit(0);
  }

  //Llenado de variables globales
  nombreBuffer = argv[1];

  //Verifica que el tamaño del buffer sea un numero
  if (!isNumber(argv[2]))
  {
    printc("<E> Debe digitar un tamaño de buffer válido y entero.\n", 1);
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
  if (Clave == -1)
  {
    printc("<E> No se consigue la clave para memoria compartida\n", 1);
    exit(0);
  }

  //Creacion del buffer
  if (createMem(&Id_Memoria, Clave, tamanoBuffer, &Memoria) == 0)
  {
    printc("<E> Error al crear el buffer\n", 1);
    exit(0);
  }

  char test[100];
  sprintf(test, "<I> Se generó el buffer %s de tamaño %d correctamente\n", nombreBuffer, tamanoBuffer);
  printc(test, 3);
  sleep(1);

  int Id_Variables = 0;
  if (VarMem(&Id_Variables, &Global) == 0)
  {
    printc("<E> Error al leer las variables globales\n", 1);
    exit(0);
  }

  printc("<I> Se genenraron correctamente las variables Globales\n", 3);
  sleep(1);

  Global[0].Tproducers = 0;
  Global[0].Tconsumers = 0;
  Global[0].consumers = 0;
  Global[0].producers = 0;
  Global[0].keyEliminated = 0;
  Global[0].totalWait = 0;
  Global[0].totalBloq = 0;
  Global[0].totalUser = 0;
  Global[0].totalKernel = 0;
  Global[0].readIndex = 0;
  Global[0].writeIndex = 0;
  Global[0].idmem = Id_Memoria;
  Global[0].totalMsg = 0;
  Global[0].memSize = tamanoBuffer;
  Global[0].endSys = 0;

  printc("<I> Variables Globales inicializazdas\n", 3);
  sleep(1);

  initializeMem(tamanoBuffer);

  printc("<I> Memoria compartida inicializazda\n", 3);
  sleep(1);

  //inicializar los semaforos
  Memc = sem_create(dirName, 10, 1, 1);
  Lleno = sem_create(dirName, 11, 1, 0);
  Vacio = sem_create(dirName, 12, 1, tamanoBuffer);

  printc("<I> Semáforos inicializazdos\n", 3);
  sleep(1);

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