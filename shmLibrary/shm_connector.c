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
    sem_t* sem;
    char* data;
} ShmData;

static off_t shmSize = 0;

shmPointer attach_shm(const char * name)
{
    size_t shmfd;
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

    shmSize = shmStats.st_size;
    
    shm = mmap(NULL, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (shm == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    close(shmfd);

    shmPointer res = malloc(sizeof(ShmData));
    res->data = (char *) shm + sizeof(sem_t);
    res->sem = (sem_t *) shm;

    return res;
}

void dettach_shm(shmPointer shm) 
{
    munmap(shm, (size_t)shmSize);
    free(shm);
}