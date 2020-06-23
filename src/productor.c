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
message *buffer= NULL;
var *Variables = NULL;
key_t bufferKey;

int numProcess = 0;
pid_t Process[5];
int state = -1;

int numMsg = 0; 
double acumWait = 0;
double acumBloq = 0;

int Memc,Lleno,Vacio;
//Funciones
void killProducer(double tiempo);

/*=====================MAIN FUNCTION================================*/
int main(int argc, char *argv[])
{
  system("clear");
  //variables para obtencion del tiempo
  struct timeval t1, t2, t3, t4, start, end;
  srand(time(NULL));

  wait = 5 * exponencial(mediaTiempo/10);

  //Verifica el numero de argumentos
  if(argc != 3){
    printc("<E> El uso correcto es:\n \t./productor <Nombre_Buffer> <Media_tiempos>\n",1);
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

  //Obtension de la media de tiempo
  if (!isNumber(argv[2])){
    printc("<E> Debe digitar un tamaño de buffer válido y entero.\n",1);
    exit(0);
  }
  mediaTiempo = atoi(argv[2]);

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
  usleep(150000);

  //Abre los semaforos que se utilizaran
  Memc=sem_open(buffdir,10,1);
  Lleno = sem_open(buffdir,11,1);
  Vacio = sem_open(buffdir,12,1);

  //Acciones del productor
  Variables[0].producers += 1;

  gettimeofday(&start, NULL); //inicia el contador de tiempo

  while (1){
    if(Variables[0].endSys){
      gettimeofday(&end, NULL);
      double tiempo = (end.tv_sec - start.tv_sec);
      killProducer(tiempo);
    }else{
      //inicio zona critica
      if(isFull(buffer, Variables[0].memSize)){
        printc("<I> La memoria se llenó\n",3);
      }

      gettimeofday(&t3, NULL);
      sem_down(Vacio,0);
      gettimeofday(&t4, NULL);

      gettimeofday(&t1, NULL);
      sem_down(Memc,0);
      gettimeofday(&t2, NULL);

      int index = freeMemSpace(buffer,Variables[0].memSize);

      time_t T= time(NULL);
      struct tm tm = *localtime(&T);
      
      buffer[index].used = 1;
      buffer[index].pid = getpid();
      buffer[index].magic_number = rand() % 6;

      char date[25];
      sprintf(date, "%02d/%02d/%04d",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
      strcpy(buffer[index].date,date);

      char hour[25];
      sprintf(hour, "%02d:%02d:%02d",tm.tm_hour, tm.tm_min, tm.tm_sec);
      strcpy(buffer[index].date,date);

      strcpy(buffer[index].msg,"Hola");

      char out[150];
      sprintf(out,"<M> Escrito un mensaje en %d:\n\t- Productores activos: %d\n\t- Consumidores activos: %d\n\n",index,Variables[0].producers,Variables[0].consumers);
      printc(out,5);
      numMsg += 1; 
    
      sem_up(Memc,0);
      sem_up(Lleno,0);
      // fin zona critica

      if(wait>0 && wait<=10){
        sleep(wait);
      }else{
        sleep(3);
      }
      
    }
    acumBloq += (t2.tv_sec - t1.tv_sec);
    acumWait += (t4.tv_sec - t3.tv_sec);
  }
  return 0;
}

/*=====================AUX FUNCTIONS================================*/

void killProducer(double tiempo){
  Variables[0].producers += 1;
  printc("<I> Se detecto una señal de finalizacion\n",2);
  usleep(500000);
  printc("<I> Resumen de actividad: \n",4);
  printc("\t- Identificador: ",4);
  printf("%d\n", getpid());
  printc("\t- No. Mensajes Producidos: ",4);
  printf("%d\n", numMsg);
  printc("\t- Tiempo en espera: ",4);
  printf("%.5f minutos\n", acumWait);
  printc("\t- Tiempo en bloqueo: ",4);
  printf("%.5f minutos\n", acumBloq);
  printc("\t- Tiempo total: ",4);
  printf("%.5f minutos\n", tiempo);
  exit(0);
}