#include <stdio.h>
#include <semaphore.h>
#include "shm_connector.h"

#define MAX_OUTPUT_LENGTH 2048

static inline int readLine(char* dst, char* src) {
    int len = 0;
    while(*src != '\n') {
        if(*src == EOF) return -1;
        dst[len++] = *src++;
    }
    dst[len++] = 0;
    return len;
}

int main(int argc, char const *argv[]) {
    ShmData shm;

    if (argc > 1)
    {
        shm = attach_shm(argv[1]);
    }
    else
    {
        char shmName[SHM_NAME_LEN];
        if(scanf("%s", shmName) == -1) {
            return 1;
        }
        shm = attach_shm(shmName);
    }

    char buf[MAX_OUTPUT_LENGTH]; int len = 0;
    char* data = shm.data;
    sem_wait(shm.sem);
    while((len = readLine(buf, data)) != -1) {
        if(len > 0) {
            data += len;
            printf("%s\n", buf);
        }

        sem_wait(shm.sem);
    }

    dettach_shm(shm);

    return 0;
}

