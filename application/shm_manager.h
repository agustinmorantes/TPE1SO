#pragma once
#include <semaphore.h>

#define SHM_NAME "/THEBIGSHM"

typedef struct 
{
    sem_t semaphore;

} ShmMemory;


void * create_shm(int fileCount);

void destroy_shm();