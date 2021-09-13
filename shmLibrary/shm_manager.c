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

#define SHM_NAME_FORMAT "/THEBIGSHM%d"

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
    snprintf(res->name, SHM_NAME_LEN, SHM_NAME_FORMAT, getpid());

    int shmfd = shm_open(res->name, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP);
    if (shmfd < 0)
    {
        free(res);
        return NULL;
    }

    if (ftruncate(shmfd,res->size) != 0)
    {
        free(res);
        return NULL;
    }

    void * shm = mmap(NULL,res->size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

    if (shm == MAP_FAILED)
    {
        free(res);
        return NULL;
    }

    if (close(shmfd) < 0)
    {
        free(res);
        return NULL;
    }

    if (sem_init((sem_t *) shm, 1, 0) < 0)
    {
        free(res);
        return NULL;
    } 

    res->memPointer = shm;
    res->data = (char *) shm + sizeof(sem_t);
    res->sem = (sem_t *) shm;

    return res;
}

char * getData(ShmPointer shm) 
{
    return shm->data;
}

int waitsem(ShmPointer shm)
{
    return sem_wait(shm->sem);
}

int postsem(ShmPointer shm)
{
    return sem_post(shm->sem);
}

int destroy_shm(ShmPointer shm) 
{
    int ret = 0;
    ret += sem_destroy(shm->sem);
    ret += munmap(shm->memPointer, shm->size);
    ret += shm_unlink(shm->name);
    free(shm);
    return ret;
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
        free(res);
        return NULL;
    }

    if (fstat(shmfd, &shmStats) == -1) 
    {
        free(res);
        return NULL;
    }
    
    res->size = shmStats.st_size;
    snprintf(res->name, SHM_NAME_LEN, "%s", name);

    shm = mmap(NULL, res->size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (shm == MAP_FAILED)
    {
        free(res);
        return NULL;
    }

    if (close(shmfd) < 0)
    {
        free(res);
        return NULL;
    }

    res->memPointer = shm;
    res->data = (char *) shm + sizeof(sem_t);
    res->sem = (sem_t *) shm;

    return res;
}

int dettach_shm(ShmPointer shm) 
{
    int ret = munmap(shm->memPointer, shm->size);
    free(shm);
    return ret;
}

char * getName(ShmPointer shm) 
{
    return shm->name;
}