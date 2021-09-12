#pragma once

#define SHM_NAME_LEN 1024

typedef struct {
    sem_t* sem;
    char* data;
} ShmData;

ShmData attach_shm(const char * name);
void dettach_shm(ShmData shm);

