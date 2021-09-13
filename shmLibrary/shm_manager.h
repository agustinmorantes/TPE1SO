#pragma once

#define SHM_NAME_LEN 1024

typedef struct ShmData * shmPointer;

shmPointer create_shm(int fileCount);

void destroy_shm();

shmPointer attach_shm(const char * name);

void dettach_shm(shmPointer shm);

char * getData(shmPointer shm) ;

void waitsem(shmPointer shm);

void postsem(shmPointer shm);