#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include "shm_manager.h"

typedef struct ShmData {
    sem_t * sem;
    char * data;
    int size;
} ShmData;

shmPointer attach_shm(const char * name)
{
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
    
    shm = mmap(NULL, shmStats.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (shm == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    close(shmfd);

    shmPointer res = malloc(sizeof(ShmData));
    res->data = (char *) shm + sizeof(sem_t);
    res->sem = (sem_t *) shm;
    res->size = shmStats.st_size;

    return res;
}

void dettach_shm(shmPointer shm) 
{
    munmap(shm, shm->size);
    free(shm);
}