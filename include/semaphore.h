#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

int sem_create(char *buffer, int identifier, int sem_num, int init_val);
int sem_erase(int id);
int sem_open(char *buffer, int identifier, int sem_num);
void sem_up(int id, int num);
void sem_down(int id, int num);
int sem_get_value(int id, int num);

#endif