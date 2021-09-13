#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include "shm_manager.h"

#define SHM_NAME "/THEBIGSHM"

typedef struct ShmData {
    sem_t * sem;
    char * data;
    int size;
} ShmData;

shmPointer create_shm(int fileCount) 
{
    int shmfd = shm_open(SHM_NAME, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP);
    if (shmfd < 0)
    {
        perror("shm_open");
        exit(1);
    }

    if (ftruncate(shmfd,PIPE_BUF*fileCount) != 0)
    {
        perror("ftruncate");
        exit(1);
    }

    void * shm = mmap(NULL,PIPE_BUF*fileCount, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

    if (shm == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    close(shmfd);
    
    sem_init((sem_t *) shm, 1, 0); 

    shmPointer res = malloc(sizeof(ShmData));
    res->data = (char *) shm + sizeof(sem_t);
    res->sem = (sem_t *) shm;
    res->size = PIPE_BUF*fileCount;

    return res;
}

char * getData(shmPointer shm) 
{
    return shm->data;
}

void waitsem(shmPointer shm)
{
    sem_wait(shm->sem);
}

void postsem(shmPointer shm)
{
    sem_post(shm->sem);
}

void destroy_shm(shmPointer shm) 
{
    sem_destroy((sem_t *) shm);
    munmap(shm, shm->size);
    shm_unlink(SHM_NAME);
    free(shm);
}

