// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <semaphore.h>
#include "../shmLibrary/shm_manager.h"

#define MAX_OUTPUT_LENGTH 1024
#define EOT 4

static inline int readLine(char* dst, char* src) {
    int len = 0;
    while(*src != '\n') {
        if(*src == EOT) return -1;
        dst[len++] = *src++;
    }
    dst[len++] = 0;
    return len;
}

int main(int argc, char const *argv[]) {
    shmPointer shm;
    if (argc > 1)
    {
        shm = attach_shm(argv[1]);
    }
    else
    {
        char shmName[SHM_NAME_LEN];
        if(scanf("%17s", shmName) == -1) {
            return 1;
        }
        shm = attach_shm(shmName);
    }

    char buf[MAX_OUTPUT_LENGTH]; int len = 0;
    char* data = getData(shm);
    
    waitsem(shm);
    while((len = readLine(buf, data)) != -1) {
        if(len > 0) {
            data += len;
            printf("%s\n", buf);
        }

        waitsem(shm);
    }

    dettach_shm(shm);

    return 0;
}

