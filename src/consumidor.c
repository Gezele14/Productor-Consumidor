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
int wait = 0;
int mode = 0;
char *deadLog;
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
void killConsumer(double tiempo, char *motivo);

/*=====================MAIN FUNCTION================================*/
int main(int argc, char *argv[])
{
  system("clear");
  //variables para obtencion del tiempo
  struct timeval t1, t2, t3, t4, start, end;

  wait = 5 * exponencial(mediaTiempo/10);

  //Verifica el numero de argumentos
  if(argc != 4){
    printc("<E> El uso correcto es:\n \t./consumidor <Nombre_Buffer> <Media_tiempos> <Modo_Operacion>\n",1);
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

  //Obtencion de la media de tiempo
  if (!isNumber(argv[2])){
    printc("<E> Debe digitar un numero de media entero.\n",1);
    exit(0);
  }
  mediaTiempo = atoi(argv[2]);

  //Obtencion del modo de operacion
  if (!isNumber(argv[3])){
    printc("<E> Debe digitar un numero de modo entero.\n",1);
    exit(0);
  }

  if (atoi(argv[3]) != 0 &&  atoi(argv[3]) != 1){
    printc("<E> Debe digitar 1 para manual y 0 para automatico.\n",1);
    exit(0);
  }
  mode = atoi(argv[3]);

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

  //Abre los semaforos que se utilizaran
  Memc = sem_open(buffdir,10,1);
  Lleno = sem_open(buffdir,11,1);
  Vacio = sem_open(buffdir,12,1);

  //Acciones del consumidor
  Variables[0].consumers += 1;

  gettimeofday(&start, NULL); //inicia el contador de tiempo

  while (1){
    if(Variables[0].endSys){
      gettimeofday(&end, NULL);
      double tiempo = (end.tv_sec - start.tv_sec);
      deadLog = "Lectura de la finalizacion del sistema.";
      killConsumer(tiempo, deadLog);
    }else{

      //inicio zona critica
      gettimeofday(&t1, NULL);
      sem_down(Lleno,0);
      gettimeofday(&t2, NULL);
      
      gettimeofday(&t3, NULL);
      sem_down(Memc,0);
      gettimeofday(&t4, NULL);

      int index = usedMemSpace(buffer, Variables[0].memSize);
      
      if(mode){
        printc("Precione una tecla para leer\n",4);
        getchar();
      }

      char text1[200];
      sprintf(text1,"<M> Leido del bloque %d:\n", index);
      printc(text1,6);

      char text2[200];
      sprintf(text2,"\t- %s|%s ",buffer[index].date, buffer[index].hour);
      printc(text2,6);

      char text3[200];
      sprintf(text3," - %s\n\t- Productores activos: %d\n\t- Consumidores activos: %d\n\n",buffer[index].msg,Variables[0].producers,Variables[0].consumers);
      printc(text3,6);

      buffer[index].used = 0;

      if(buffer[index].magic_number == getpid()%6){
        gettimeofday(&end, NULL);
        double tiempo = (end.tv_sec - start.tv_sec);
        deadLog = "PID % 6 coincidió con el número mágico.";
        sem_up(Memc,0);
        sem_up(Vacio,0);
        killConsumer(tiempo, deadLog);
      }

      sem_up(Memc,0);
      sem_up(Vacio,0);
      // fin zona critica

      numMsg += 1;

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

void killConsumer(double tiempo, char *motivo){
  Variables[0].consumers -= 1;
  printc("<I> Se detecto una señal de finalizacion\n",2);
  usleep(500000);
  printc("<I> Resumen de actividad: \n",4);
  printc("\t- Identificador: ",4);
  printf("%d\n", getpid());
  printc("\t- Razón de suspensión: ",4);
  printf("%s\n", deadLog);
  printc("\t- No. Mensajes Consumidos: ",4);
  printf("%d\n", numMsg);
  printc("\t- Tiempo en espera: ",4);
  printf("%.2f minutos\n", acumWait/60);
  printc("\t- Tiempo en bloqueo: ",4);
  printf("%.2f minutos\n", acumBloq/60);
  printc("\t- Tiempo total: ",4);
  printf("%.2f minutos\n", tiempo/60);
  exit(0);
}