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

void * create_shm(int fileCount) {
    int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP);
    if (fd < 0)
    {
        perror("shm_open");
        exit(1);
    }

    if (ftruncate(fd,PIPE_BUF*fileCount) != 0)
    {
        perror("ftruncate");
        exit(1);
    }

    void * shm = mmap(NULL,PIPE_BUF*fileCount, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (shm == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    close(fd);
    
    sem_init((sem_t *) shm, 1, 1); 

    return shm;
}

void destroy_shm(void * shm) 
{
    sem_destroy((sem_t *) shm);
    munmap(shm, sizeof(shm));
    shm_unlink(SHM_NAME);
}