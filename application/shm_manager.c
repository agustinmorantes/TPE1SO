// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

static size_t shmSize = 0;

void * create_shm(int fileCount) 
{
    int shmfd = shm_open(SHM_NAME, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP);
    if (shmfd < 0)
    {
        perror("shm_open");
        exit(1);
    }

    shmSize = PIPE_BUF*fileCount;

    if (ftruncate(shmfd,shmSize) != 0)
    {
        perror("ftruncate");
        exit(1);
    }

    void * shm = mmap(NULL, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

    if (shm == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    close(shmfd);
    
    sem_init((sem_t *) shm, 1, 0); 

    return shm;
}

void destroy_shm(void * shm) 
{
    sem_destroy((sem_t *) shm);
    munmap(shm, shmSize);
    shm_unlink(SHM_NAME);
}