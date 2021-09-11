#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>

#define SHM_NAME "/THEBIGSHM"

char * create_shm(int fileCount) {
    int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP);
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

    char * shm = (char *) mmap(NULL,PIPE_BUF*fileCount, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (shm == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    close(fd);
    
    printf("%s",SHM_NAME);

    return shm;
}