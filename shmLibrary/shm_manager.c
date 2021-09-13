#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <semaphore.h>
#include "shm_manager.h"

typedef struct ShmData {
    void * memPointer;
    sem_t * sem;
    char * data;
    int size;
    char name[SHM_NAME_LEN];
} ShmData;

ShmPointer create_shm(int dataSize) 
{
    ShmPointer res = malloc(sizeof(ShmData));
    res->size =  dataSize + sizeof(sem_t);
    snprintf(res->name, SHM_NAME_LEN,"\\THEBIGSHM%d", getpid());

    int shmfd = shm_open(res->name, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP);
    if (shmfd < 0)
    {
        perror("shm_open");
        exit(1);
    }

    if (ftruncate(shmfd,res->size) != 0)
    {
        perror("ftruncate");
        exit(1);
    }

    void * shm = mmap(NULL,res->size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

    if (shm == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    close(shmfd);

    sem_init((sem_t *) shm, 1, 0); 

    res->memPointer = shm;
    res->data = (char *) shm + sizeof(sem_t);
    res->sem = (sem_t *) shm;

    return res;
}

char * getData(ShmPointer shm) 
{
    return shm->data;
}

void waitsem(ShmPointer shm)
{
    sem_wait(shm->sem);
}

void postsem(ShmPointer shm)
{
    sem_post(shm->sem);
}

void destroy_shm(ShmPointer shm) 
{
    sem_destroy(shm->sem);
    munmap(shm->memPointer, shm->size);
    shm_unlink(shm->name);
    free(shm);
}

ShmPointer attach_shm(const char * name)
{
    ShmPointer res = malloc(sizeof(ShmData));
    int shmfd;
    struct stat shmStats;
    void * shm;

    shmfd = shm_open(name, O_RDWR, 0);
    if (shmfd < 0) 
    {
        perror("shm_open");
        exit(1);
    }

    if (fstat(shmfd, &shmStats) == -1) 
    {
        perror("fstat");
        exit(1);
    }
    
    res->size = shmStats.st_size;
    snprintf(res->name, SHM_NAME_LEN, "%s", name);

    shm = mmap(NULL, res->size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (shm == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    close(shmfd);

    res->memPointer = shm;
    res->data = (char *) shm + sizeof(sem_t);
    res->sem = (sem_t *) shm;

    return res;
}

void dettach_shm(ShmPointer shm) 
{
    munmap(shm->memPointer, shm->size);
    free(shm);
}

char * getName(ShmPointer shm) 
{
    return shm->name;
}