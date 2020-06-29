/**
 * Basado en el Tutorial de:
 * C.U. Elena y S.R Ranquel
 * URL: https://www2.infor.uva.es/~cllamas/concurr/pract97/rsantos/index.html
 * 
 * chuidiang.org
 * URL: http://www.chuidiang.org/clinux/ipcs/mem_comp.php
*/
/*=====================IMPORTS================================*/
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

/*=====================VARIABLES================================*/
//Variables
int mediaTiempo = 0;
int wait;
message *buffer = NULL;
var *Variables = NULL;
key_t bufferKey;

int numProcess = 0;
pid_t Process[5];
int state = -1;

int numMsg = 0;
double acumWait = 0;
double acumBloq = 0;

char *msg[5] = {"Gato", "Perro", "Loro", "Mono", "Aguila"};

int Memc, Lleno, Vacio;
//Funciones
void killProducer(double tiempo);

/*=====================MAIN FUNCTION================================*/
int main(int argc, char *argv[])
{
  system("clear");
  printc("Productor | Productor-Consumidor\n\n", 6);
  //variables para obtencion del tiempo
  struct timeval t1, t2, t3, t4, start, end;
  srand(time(NULL));

  wait = 5 * exponencial(mediaTiempo / 10);

  //Verifica el numero de argumentos
  if (argc != 3)
  {
    printc("<E> El uso correcto es:\n \t./productor <Nombre_Buffer> <Media_tiempos>\n", 1);
    exit(0);
  }

  //Creacion de la clave para el buffer
  char *buffdir = concat("buffers/", argv[1]);
  if (!chkDir(buffdir))
  {
    printc("<E> El buffer no existe\n", 1);
    exit(0);
  }
  else
  {
    bufferKey = ftok(buffdir, 's');
    if (bufferKey == -1)
    {
      printc("<E> Error al consiguir la clave para memoria compartida\n", 1);
      exit(0);
    }
  }

  //Obtension de la media de tienuoermda
  if (!isNumber(argv[2]))
  {
    printc("<E> Debe digitar un numero de media entero.\n", 1);
    exit(0);
  }
  mediaTiempo = atoi(argv[2]);

  //obtencion de las variables globales.
  int idVar = 0;
  if (VarMem(&idVar, &Variables) == 0)
  {
    printc("<E> Error al leer las variables globales\n", 1);
    exit(0);
  }

  printc("<I> Variables Globales cargadas correctamente\n", 3);
  sleep(1);

  int idmem;
  if (!createMem(&idmem, bufferKey, Variables[0].memSize, &buffer))
  {
    printc("<E> Error al obtener el buffer\n", 1);
    exit(0);
  }

  if (!(idmem == Variables[0].idmem))
  {
    printc("<E> El Buffer aun no se a inicializado\n", 1);
    exit(0);
  }

  printc("<I> Buffer cargado correctamente\n", 3);
  usleep(150000);

  //Abre los semaforos que se utilizaran
  Memc = sem_open(buffdir, 10, 1);
  Lleno = sem_open(buffdir, 11, 1);
  Vacio = sem_open(buffdir, 12, 1);

  //Acciones del productor
  Variables[0].producers += 1;
  Variables[0].Tproducers += 1;

  gettimeofday(&start, NULL); //inicia el contador de tiempo

  while (1)
  {
    //inicio zona critica
    if (isFull(buffer, Variables[0].memSize))
    {
      printc("<I> La memoria se llenó\n", 3);
    }

    gettimeofday(&t1, NULL);
    sem_down(Vacio, 0);
    gettimeofday(&t2, NULL);

    gettimeofday(&t3, NULL);
    sem_down(Memc, 0);
    gettimeofday(&t4, NULL);

    if (Variables[0].endSys == 2)
    {
      gettimeofday(&end, NULL);
      double tiempo = (end.tv_sec - start.tv_sec);
      Variables[0].totalKernel += (end.tv_sec - start.tv_sec);
      killProducer(tiempo);
    }

    numMsg += 1;

    Variables[0].writeIndex = freeMemSpace(buffer, Variables[0].memSize, Variables[0].writeIndex);

    time_t T = time(NULL);
    struct tm tm = *localtime(&T);

    buffer[Variables[0].writeIndex].used = 1;
    buffer[Variables[0].writeIndex].magic_number = rand() % 6;

    char date[25];
    sprintf(date, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(buffer[Variables[0].writeIndex].date, date);

    char hour[25];
    sprintf(hour, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    strcpy(buffer[Variables[0].writeIndex].hour, hour);

    strcpy(buffer[Variables[0].writeIndex].msg, msg[rand() % 5]);

    char out[150];
    sprintf(out, "<M> Escrito un mensaje en %d:\n\t- Productores activos: %d\n\t- Consumidores activos: %d\n\n", Variables[0].writeIndex, Variables[0].producers, Variables[0].consumers);
    printc(out, 5);

    Variables[0].totalMsg += 1;

    acumBloq += (t4.tv_sec - t3.tv_sec);
    acumWait += (t2.tv_sec - t1.tv_sec);

    Variables[0].totalBloq += (t2.tv_sec - t1.tv_sec);
    Variables[0].totalWait += (t4.tv_sec - t3.tv_sec);

    if (wait > 1 && wait <= 10)
    {
      sleep(wait);
    }
    else
    {
      sleep(3);
    }

    Variables[0].writeIndex += 1;

    sem_up(Memc, 0);
    sem_up(Lleno, 0);
    // fin zona critica
  }
  return 0;
}

/*=====================AUX FUNCTIONS================================*/

void killProducer(double tiempo)
{
  Variables[0].producers -= 1;
  printc("<I> Se detecto una señal de finalizacion\n", 2);
  usleep(500000);
  printc("<I> Resumen de actividad: \n", 4);
  printc("\t- Identificador: ", 4);
  printf("%d\n", getpid());
  printc("\t- No. Mensajes Producidos: ", 4);
  printf("%d\n", numMsg);
  printc("\t- Tiempo en espera: ", 4);
  printf("%.2f minutos\n", acumWait / 60);
  printc("\t- Tiempo en bloqueo: ", 4);
  printf("%.2f minutos\n", acumBloq / 60);
  printc("\t- Tiempo total: ", 4);
  printf("%.2f minutos\n", tiempo / 60);
  exit(0);
}