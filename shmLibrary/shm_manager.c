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

shmPointer create_shm(int dataSize) 
{
    shmPointer res = malloc(sizeof(ShmData));
    snprintf(res->name, SHM_NAME_LEN,"\\THEBIGSHM%d", getpid());
    res->size =  dataSize + sizeof(sem_t);

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
    sem_destroy(shm->sem);
    munmap(shm->memPointer, shm->size);
    shm_unlink(shm->name);
    free(shm);
}

shmPointer attach_shm(const char * name)
{
    shmPointer res = malloc(sizeof(ShmData));
    snprintf(res->name, SHM_NAME_LEN, "%s", name);
    int shmfd;
    struct stat shmStats;
    void * shm;

    shmfd = shm_open(res->name, O_RDWR, 0);
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

void dettach_shm(shmPointer shm) 
{
    munmap(shm->memPointer, shm->size);
    free(shm);
}

char * getName(shmPointer shm) 
{
    return shm->name;
}