#pragma once

#define SHM_NAME_LEN 18

typedef struct ShmData * ShmPointer;

ShmPointer create_shm(int dataSize);
int destroy_shm();

ShmPointer attach_shm(const char * name);
int dettach_shm(ShmPointer shm);

char * getData(ShmPointer shm) ;

int waitsem(ShmPointer shm);
int postsem(ShmPointer shm);

char * getName(ShmPointer shm);
