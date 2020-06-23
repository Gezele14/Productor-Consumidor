#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>


struct sembuf operation;
/**
 * Creates a semaphore 
 * char* buffer --> key path
 * int identifier --> identifier
 * int sem_num --> number of semaphores
 * int init_val --> initial value of the set of semaphores, 0 is red and 1 is green
 * Example: sem_create("../bin/ls",33, 10,1);
*/
int sem_create(char *buffer, int identifier, int sem_num, int init_val) {
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
// The union is already defined in sys/sem.h
#else
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short int *array;
        struct seminfo *__buf;
    };
#endif

    
    union semun arg;
    int id;
    key_t key;

    key = ftok(buffer, identifier);

    if (key == -1)
    {
        printf("No se pudo obtener la llave del semaforo\n");
        exit(0);
    }

    id = semget(key, sem_num, 0600 | IPC_CREAT);

    if (id == -1)
    {
        printf("No se pudo crear el semaforo\n");
        exit(0);
    }

    for (int i = 0; i <= sem_num; i++)
    {
      arg.val = init_val;
      semctl(id,i, SETVAL, arg);
    } 

    operation.sem_flg = 0;

    return id;
}

/**
 * Erase a set semaphore 
 * int id --> id of the set of semaphores
*/
int sem_erase(int id) {
    if (shmctl(id, IPC_RMID, 0) < 0) {
        return 0;
    } else
        return 1;
}

/**
 * Open an existing semaphore 
 * char* buffer --> key path
 * int identifier --> identifier
 * int sem_num --> number of semaphores
 * Example: sem_open("../bin/ls",33, 10);
*/

int sem_open(char *buffer, int identifier, int sem_num) {
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
// The union is already defined in sys/sem.h
#else
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short int *array;
        struct seminfo *__buf;
    };
#endif

    //union semun arg;
    int id;
    key_t key;

    key = ftok(buffer, identifier);

    if (key == -1)
    {
        printf("No se pudo obtener la llave del semaforo(abrir)\n");
        exit(0);
    }

    id = semget(key, sem_num, 0600);

    if (id == -1)
    {
        printf("No se pudo crear el semaforo(abrir)\n");
        exit(0);
    }

    operation.sem_flg = 0;

    return id;
}

/**
 * Up operation 
 * int id --> id of the set of semaphores
 * int num --> number of the especific semaphore
 * Example: sem_up(sem1,0);
*/
void sem_up(int id, int num) {
    struct sembuf operation;

    operation.sem_num = num;
    operation.sem_op = 1;
    semop(id, &operation, 1);
}
/**
 * Down operation 
 * int id --> id of the set of semaphores
 * int num --> number of the especific semaphore
 * Example: sem_down(sem1,0);
*/
void sem_down(int id, int num) {
    struct sembuf operation;

    operation.sem_num = num;
    operation.sem_op = -1;
    semop(id, &operation, 1);
}

/**
 * Get the value of the especific semaphore 
 * int id --> id of the set of semaphores
 * int num --> number of the especific semaphore
 * Example: sem_get_value(sem1,0);
*/
int sem_get_value(int id, int num) {
    int value;
    value = semctl(id, num, GETVAL, 0);
    return value;
}
