#pragma once

#define SHM_NAME_LEN 18

typedef struct ShmData * ShmPointer;

ShmPointer create_shm(int dataSize);
void destroy_shm();

ShmPointer attach_shm(const char * name);
void dettach_shm(ShmPointer shm);

char * getData(ShmPointer shm) ;

void waitsem(ShmPointer shm);
void postsem(ShmPointer shm);

char * getName(ShmPointer shm);
