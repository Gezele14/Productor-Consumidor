/**
 * Funcion Inicializador del Productor-Consumidor
 * Made by: @gezele14 
 * Miercóles 17 de junio, 2020
*/

//Imports
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>

#include <util.h>

//Variables globales
char *nombreBuffer = "";
int *tamanoBuffer;

//Para la memoria compartida
  key_t Clave;
	int Id_Memoria;
  int *Memoria = NULL;

//Funcion Principal
int main(int argc, char *argv[]){
  //Verifica cantidad de argumentos.
  if (argc != 3){
    printf("Uso correcto:\n\t ./main <Nombre_Buffer> <Tamaño_Buffer>\n");
    exit(0);
  }

  //Llenado de variables globales 
  nombreBuffer = argv[1];
  tamanoBuffer = (int *)malloc(sizeof(int));

  //Verifica que el tamaño del buffer sea un numero
  if (!isNumber(argv[2])){
    printf("Debe digitar un tamaño de buffer válido y entero.\n");
    exit(0);
  }
  *tamanoBuffer = atoi(argv[2]);

  //verifica si el directorio existe y si no lo crea
  char * dirName = concat("../buffers/",nombreBuffer);
  if (!chkDir(dirName)){
    createDir(dirName);
  }

  //Obtension de la clave
  Clave = ftok (dirName, 33);
	if (Clave == -1){
		printf("No se consigue la clave para memoria compartida\n");
		exit(0);
	}

  //Creacion del buffer
  Id_Memoria = shmget (Clave, sizeof(int)*100, 0777 | IPC_CREAT);
	if (Id_Memoria == -1){
		printf("No consigo Id para memoria compartida\n");
		exit (0);
	}

  Memoria = (int *)shmat (Id_Memoria, (char *)0, 0);
	if (Memoria == NULL){
		printf("No consigo memoria compartida\n");
		exit (0);
	}

  for (int i=0; i<10; i++){
		for (int j=0; j<100; j++){
			Memoria[j] = i;
		}
		printf("Escrito %d\n",i);
		sleep (1);
	}

  shmdt ((char *)Memoria);
	shmctl (Id_Memoria, IPC_RMID, (struct shmid_ds *)NULL);

  free(tamanoBuffer);
  return 0;
}